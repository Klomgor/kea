// Copyright (C) 2012-2025 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <asiolink/io_service.h>
#include <asiolink/process_spawn.h>
#include <cc/default_credentials.h>
#include <database/database_connection.h>
#include <database/db_log.h>
#include <exceptions/exceptions.h>
#include <mysql/mysql_connection.h>
#include <util/filesystem.h>

#include <boost/lexical_cast.hpp>

#include <cstdint>
#include <exception>
#include <limits>
#include <string>
#include <unordered_map>

using namespace isc;
using namespace isc::asiolink;
using namespace isc::data;
using namespace std;

namespace isc {
namespace db {

std::string MySqlConnection::KEA_ADMIN_ = KEA_ADMIN;

int MySqlHolder::atexit_ = [] {
    return atexit([] { mysql_library_end(); });
}();

/// @todo: Migrate this default value to src/bin/dhcpX/simple_parserX.cc
const int MYSQL_DEFAULT_CONNECTION_TIMEOUT = 5; // seconds

MySqlTransaction::MySqlTransaction(MySqlConnection& conn)
    : conn_(conn), committed_(false) {
    conn_.startTransaction();
}

MySqlTransaction::~MySqlTransaction() {
    // Rollback if the MySqlTransaction::commit wasn't explicitly
    // called.
    if (!committed_) {
        conn_.rollback();
    }
}

void
MySqlTransaction::commit() {
    conn_.commit();
    committed_ = true;
}

// Open the database using the parameters passed to the constructor.

void
MySqlConnection::openDatabase() {
    // Set up the values of the parameters
    const char* host = "localhost";
    string shost;
    try {
        shost = getParameter("host");
        host = shost.c_str();
    } catch (...) {
        // No host.  Fine, we'll use "localhost"
    }

    unsigned int port = 0;
    try {
        setIntParameterValue("port", 0, numeric_limits<uint16_t>::max(), port);

    } catch (const std::exception& ex) {
        isc_throw(DbInvalidPort, ex.what());
    }

    const char* user = NULL;
    string suser;
    try {
        suser = getParameter("user");
        user = suser.c_str();
    } catch (...) {
        // No user.  Fine, we'll use NULL
    }

    const char* password = NULL;
    string spassword;
    try {
        spassword = getParameter("password");
        password = spassword.c_str();
    } catch (...) {
        // No password.  Fine, we'll use NULL
    }
    if (password) {
        // Refuse default password.
        DefaultCredentials::check(spassword);
    }

    const char* name = NULL;
    string sname;
    try {
        sname = getParameter("name");
        name = sname.c_str();
    } catch (...) {
        // No database name.  Throw a "NoName" exception
        isc_throw(NoDatabaseName, "must specify a name for the database");
    }

    unsigned int connect_timeout = MYSQL_DEFAULT_CONNECTION_TIMEOUT;
    unsigned int read_timeout = 0;
    unsigned int write_timeout = 0;
    try {
        // The timeout is only valid if greater than zero, as depending on the
        // database, a zero timeout might signify something like "wait
        // indefinitely".
        setIntParameterValue("connect-timeout", 1, numeric_limits<int>::max(), connect_timeout);
        // Other timeouts can be 0, meaning that the database client will follow a default
        // behavior. Earlier MySQL versions didn't have these parameters, so we allow 0
        // to skip setting them.
        setIntParameterValue("read-timeout", 0, numeric_limits<int>::max(), read_timeout);
        setIntParameterValue("write-timeout", 0, numeric_limits<int>::max(), write_timeout);

    } catch (const std::exception& ex) {
        isc_throw(DbInvalidTimeout, ex.what());
    }

    const char* ca_file(0);
    const char* ca_dir(0);
    string sca;
    try {
        sca = getParameter("trust-anchor");
        tls_ = true;
        if (util::file::isDir(sca)) {
            ca_dir = sca.c_str();
        } else {
            ca_file = sca.c_str();
        }
    } catch (...) {
        // No trust anchor
    }

    const char* cert_file(0);
    string scert;
    try {
        scert = getParameter("cert-file");
        tls_ = true;
        cert_file = scert.c_str();
    } catch (...) {
        // No client certificate file
    }

    const char* key_file(0);
    string skey;
    try {
        skey = getParameter("key-file");
        tls_ = true;
        key_file = skey.c_str();
    } catch (...) {
        // No private key file
    }

    const char* cipher_list(0);
    string scipher;
    try {
        scipher = getParameter("cipher-list");
        tls_ = true;
        cipher_list = scipher.c_str();
    } catch (...) {
        // No cipher list
    }

    // Set options for the connection:
    //
    int result;
#ifdef HAS_MYSQL_OPT_RECONNECT
    // Though still supported by Mariadb (as of 11.5.0), MYSQL_OPT_RECONNECT is
    // deprecated as of MySQL 8.0.34. Where it is still supported we should
    // continue to ensure it is off. Enabling it leaves us with an unusable
    // connection after a reconnect as among other things, it drops all our
    // pre-compiled statements.
    my_bool auto_reconnect = MLM_FALSE;
    result = mysql_options(mysql_, MYSQL_OPT_RECONNECT, &auto_reconnect);
    if (result != 0) {
        isc_throw(DbOpenError, "unable to set auto-reconnect option: " <<
                  mysql_error(mysql_));
    }
#endif

    // Make sure we have a large idle time window ... say 30 days...
    const char *wait_time = "SET SESSION wait_timeout = 30 * 86400";
    result = mysql_options(mysql_, MYSQL_INIT_COMMAND, wait_time);
    if (result != 0) {
        isc_throw(DbOpenError, "unable to set wait_timeout " <<
                  mysql_error(mysql_));
    }

    // Set SQL mode options for the connection:  SQL mode governs how what
    // constitutes insertable data for a given column, and how to handle
    // invalid data.  We want to ensure we get the strictest behavior and
    // to reject invalid data with an error.
    const char *sql_mode = "SET SESSION sql_mode ='STRICT_ALL_TABLES'";
    result = mysql_options(mysql_, MYSQL_INIT_COMMAND, sql_mode);
    if (result != 0) {
        isc_throw(DbOpenError, "unable to set SQL mode options: " <<
                  mysql_error(mysql_));
    }

    // Connection timeout, the amount of time taken for the client to drop
    // the connection if the server is not responding.
    result = mysql_options(mysql_, MYSQL_OPT_CONNECT_TIMEOUT, &connect_timeout);
    if (result != 0) {
        isc_throw(DbOpenError, "unable to set database connection timeout: " <<
                  mysql_error(mysql_));
    }

    // Set the read timeout if it has been specified. Otherwise, the timeout is
    // not used.
    if (read_timeout > 0) {
        result = mysql_options(mysql_, MYSQL_OPT_READ_TIMEOUT, &read_timeout);
        if (result != 0) {
            isc_throw(DbOpenError, "unable to set database read timeout: " <<
                      mysql_error(mysql_));
        }
    }

    // Set the write timeout if it has been specified. Otherwise, the timeout
    // is not used.
    if (write_timeout > 0) {
        result = mysql_options(mysql_, MYSQL_OPT_WRITE_TIMEOUT, &write_timeout);
        if (result != 0) {
            isc_throw(DbOpenError, "unable to set database write timeout: " <<
                      mysql_error(mysql_));
        }
    }

    // If TLS is enabled set it. If something should go wrong it will happen
    // later at the mysql_real_connect call.
    if (tls_) {
        result = mysql_options(mysql_, MYSQL_OPT_SSL_KEY, key_file);
        if (result != 0) {
            isc_throw(DbOpenError, "unable to set key: " << mysql_error(mysql_));
        }

        result = mysql_options(mysql_, MYSQL_OPT_SSL_CERT, cert_file);
        if (result != 0) {
            isc_throw(DbOpenError, "unable to set certificate: " << mysql_error(mysql_));
        }

        result = mysql_options(mysql_, MYSQL_OPT_SSL_CA, ca_file);
        if (result != 0) {
            isc_throw(DbOpenError, "unable to set CA: " << mysql_error(mysql_));
        }

        result = mysql_options(mysql_, MYSQL_OPT_SSL_CAPATH, ca_dir);
        if (result != 0) {
            isc_throw(DbOpenError, "unable to set CA path: " << mysql_error(mysql_));
        }

        result = mysql_options(mysql_, MYSQL_OPT_SSL_CIPHER, cipher_list);
        if (result != 0) {
            isc_throw(DbOpenError, "unable to set cipher: " << mysql_error(mysql_));
        }
    }

    // Open the database.
    //
    // The option CLIENT_FOUND_ROWS is specified so that in an UPDATE,
    // the affected rows are the number of rows found that match the
    // WHERE clause of the SQL statement, not the rows changed.  The reason
    // here is that MySQL apparently does not update a row if data has not
    // changed and so the "affected rows" (retrievable from MySQL) is zero.
    // This makes it hard to distinguish whether the UPDATE changed no rows
    // because no row matching the WHERE clause was found, or because a
    // row was found but no data was altered.
    MYSQL* status = mysql_real_connect(mysql_, host, user, password, name,
                                       port, NULL, CLIENT_FOUND_ROWS);
    if (status != mysql_) {
        // Mark this connection as no longer usable.
        markUnusable();

        std::string error_message = mysql_error(mysql_);

        auto const& rec = reconnectCtl();
        if (rec && DatabaseConnection::retry_) {

            // Start the connection recovery.
            startRecoverDbConnection();

            std::ostringstream s;

            s << " (scheduling retry " << rec->retryIndex() + 1 << " of " << rec->maxRetries() << " in " << rec->retryInterval() << " milliseconds)";

            error_message += s.str();

            isc_throw(DbOpenErrorWithRetry, error_message);
        }

        isc_throw(DbOpenError, error_message);
    }

    // Enable autocommit. In case transaction is explicitly used, this
    // setting will be overwritten for the transaction. However, there are
    // cases when lack of autocommit could cause transactions to hang
    // until commit or rollback is explicitly called. This already
    // caused issues for some unit tests which were unable to cleanup
    // the database after the test because of pending transactions.
    // Use of autocommit will eliminate this problem.
    my_bool autocommit_result = mysql_autocommit(mysql_, 1);
    if (autocommit_result != 0) {
        isc_throw(DbOperationError, mysql_error(mysql_));
    }

    // To avoid a flush to disk on every commit, the global parameter
    // innodb_flush_log_at_trx_commit should be set to 2. This will cause the
    // changes to be written to the log, but flushed to disk in the background
    // every second. Setting the parameter to that value will speed up the
    // system, but at the risk of losing data if the system crashes.
}

// Get schema version.

std::pair<uint32_t, uint32_t>
MySqlConnection::getVersion(const ParameterMap& parameters,
                            const IOServiceAccessorPtr& ac,
                            const DbCallback& cb,
                            const string& timer_name,
                            unsigned int id) {
    // Get a connection.
    MySqlConnection conn(parameters, ac, cb);

    if (!timer_name.empty()) {
        conn.makeReconnectCtl(timer_name, id);
    }

    // Open the database.
    conn.openDatabase();

    // Allocate a new statement.
    MYSQL_STMT *stmt = mysql_stmt_init(conn.mysql_);
    if (stmt == NULL) {
        isc_throw(DbOperationError, "unable to allocate MySQL prepared "
                "statement structure, reason: " << mysql_error(conn.mysql_));
    }

    try {

        // Prepare the statement from SQL text.
        const char* version_sql = "SELECT version, minor FROM schema_version";
        int status = mysql_stmt_prepare(stmt, version_sql, strlen(version_sql));
        if (status != 0) {
            isc_throw(DbOperationError, "unable to prepare MySQL statement <"
                      << version_sql << ">, reason: "
                      << mysql_error(conn.mysql_));
        }

        // Execute the prepared statement.
        if (MysqlExecuteStatement(stmt) != 0) {
            isc_throw(DbOperationError, "cannot execute schema version query <"
                      << version_sql << ">, reason: "
                      << mysql_errno(conn.mysql_));
        }

        // Bind the output of the statement to the appropriate variables.
        MYSQL_BIND bind[2];
        memset(bind, 0, sizeof(bind));

        uint32_t version;
        bind[0].buffer_type = MYSQL_TYPE_LONG;
        bind[0].is_unsigned = 1;
        bind[0].buffer = &version;
        bind[0].buffer_length = sizeof(version);

        uint32_t minor;
        bind[1].buffer_type = MYSQL_TYPE_LONG;
        bind[1].is_unsigned = 1;
        bind[1].buffer = &minor;
        bind[1].buffer_length = sizeof(minor);

        if (mysql_stmt_bind_result(stmt, bind)) {
            isc_throw(DbOperationError, "unable to bind result set for <"
                      << version_sql << ">, reason: "
                      << mysql_errno(conn.mysql_));
        }

        // Fetch the data.
        if (mysql_stmt_fetch(stmt)) {
            isc_throw(DbOperationError, "unable to bind result set for <"
                      << version_sql << ">, reason: "
                      << mysql_errno(conn.mysql_));
        }

        // Discard the statement and its resources
        mysql_stmt_close(stmt);
        return (std::make_pair(version, minor));

    } catch (const std::exception&) {
        // Avoid a memory leak on error.
        mysql_stmt_close(stmt);

        // Send the exception to the caller.
        throw;
    }
}

void
MySqlConnection::ensureSchemaVersion(const ParameterMap& parameters,
                                     const DbCallback& cb,
                                     const string& timer_name) {
    // retry-on-startup?
    bool const retry(parameters.count("retry-on-startup") &&
                     parameters.at("retry-on-startup") == "true");

    IOServiceAccessorPtr ac(new IOServiceAccessor(&DatabaseConnection::getIOService));
    pair<uint32_t, uint32_t> schema_version;
    try {
        schema_version = getVersion(parameters, ac, cb, retry ? timer_name : string());
    } catch (DbOpenError const& exception) {
        throw;
    } catch (DbOpenErrorWithRetry const& exception) {
        throw;
    } catch (exception const& exception) {
        // Disable the recovery mechanism in test mode.
        if (DatabaseConnection::test_mode_) {
            throw;
        }
        // This failure may occur for a variety of reasons. We are looking at
        // initializing schema as the only potential mitigation. We could narrow
        // down on the error that would suggest an uninitialized schema
        // which would sound something along the lines of
        // "table schema_version does not exist", but we do not necessarily have
        // to. If the error had another cause, it will fail again during
        // initialization or during the subsequent version retrieval and that is
        // fine, and the error should still be relevant.
        initializeSchema(parameters);

        // Retrieve again because the initial retrieval failed.
        schema_version = getVersion(parameters, ac, cb, retry ? timer_name : string());
    }

    // Check that the versions match.
    pair<uint32_t, uint32_t> const expected_version(MYSQL_SCHEMA_VERSION_MAJOR,
                                                    MYSQL_SCHEMA_VERSION_MINOR);
    if (schema_version != expected_version) {
        isc_throw(DbOpenError, "MySQL schema version mismatch: expected version: "
                                   << expected_version.first << "." << expected_version.second
                                   << ", found version: " << schema_version.first << "."
                                   << schema_version.second);
    }
}

void
MySqlConnection::initializeSchema(const ParameterMap& parameters) {
    if (parameters.count("readonly") && parameters.at("readonly") == "true") {
        // The readonly flag is historically used for host backends. Still, if
        // enabled, it is a strong indication that we should not meDDLe with it.
        return;
    }

    if (!isc::util::file::isFile(KEA_ADMIN_)) {
        // It can happen for kea-admin to not exist, especially with
        // packages that install it in a separate package.
        return;
    }

    // Convert parameters.
    vector<string> kea_admin_parameters(toKeaAdminParameters(parameters));
    ProcessEnvVars const vars;
    kea_admin_parameters.insert(kea_admin_parameters.begin(), "db-init");

    // Run.
    ProcessSpawn kea_admin(ProcessSpawn::SYNC, KEA_ADMIN_, kea_admin_parameters, vars,
                           /* inherit_env = */ true);
    DB_LOG_INFO(MYSQL_INITIALIZE_SCHEMA).arg(kea_admin.getCommandLine());
    pid_t const pid(kea_admin.spawn());
    if (kea_admin.isRunning(pid)) {
        isc_throw(SchemaInitializationFailed, "kea-admin still running");
    }
    int const exit_code(kea_admin.getExitStatus(pid));
    if (exit_code != 0) {
        isc_throw(SchemaInitializationFailed, "Expected exit code 0 for kea-admin. Got " << exit_code);
    }
}

vector<string>
MySqlConnection::toKeaAdminParameters(ParameterMap const& params) {
    vector<string> result{"mysql"};
    for (auto const& p : params) {
        string const& keyword(p.first);
        string const& value(p.second);

        // These Kea parameters are the same as the kea-admin parameters.
        if (keyword == "user" ||
            keyword == "password" ||
            keyword == "host" ||
            keyword == "port" ||
            keyword == "name") {
            result.push_back("--" + keyword);
            result.push_back(value);
            continue;
        }

        // These Kea parameters do not have a direct kea-admin equivalent.
        // But they do have a mariadb client flag equivalent.
        // We pass them to kea-admin using the --extra flag.
        static unordered_map<string, string> conversions{
            {"connect-timeout", "connect_timeout"},
            {"cipher-list", "ssl-cipher"},
            {"cert-file", "ssl-cert"},
            {"key-file", "ssl-key"},
            {"trust-anchor", "ssl-ca"},
            // {"read-timeout", "--net-read-timeout"},  // available in docs, but client says unknown variable?
            // {"write-timeout", "--net-write-timeout"},  // available in docs, but client says unknown variable?
        };
        if (conversions.count(keyword)) {
            result.push_back("--extra");
            result.push_back("--" + conversions.at(keyword) + " " + value);
        }
    }
    return result;
}

// Prepared statement setup.  The textual form of an SQL statement is stored
// in a vector of strings (text_statements_) and is used in the output of
// error messages.  The SQL statement is also compiled into a "prepared
// statement" (stored in statements_), which avoids the overhead of compilation
// during use.  As prepared statements have resources allocated to them, the
// class destructor explicitly destroys them.

void
MySqlConnection::prepareStatement(uint32_t index, const char* text) {
    // Validate that there is space for the statement in the statements array
    // and that nothing has been placed there before.
    if ((index >= statements_.size()) || (statements_[index] != NULL)) {
        isc_throw(InvalidParameter, "invalid prepared statement index (" <<
                  static_cast<int>(index) << ") or indexed prepared " <<
                  "statement is not null");
    }

    // All OK, so prepare the statement
    text_statements_[index] = std::string(text);
    statements_[index] = mysql_stmt_init(mysql_);
    if (statements_[index] == NULL) {
        isc_throw(DbOperationError, "unable to allocate MySQL prepared "
                  "statement structure, reason: " << mysql_error(mysql_));
    }

    int status = mysql_stmt_prepare(statements_[index], text, strlen(text));
    if (status != 0) {
        isc_throw(DbOperationError, "unable to prepare MySQL statement <" <<
                  text << ">, reason: " << mysql_error(mysql_));
    }
}

void
MySqlConnection::prepareStatements(const TaggedStatement* start_statement,
                                   const TaggedStatement* end_statement) {
    // Created the MySQL prepared statements for each DML statement.
    for (const TaggedStatement* tagged_statement = start_statement;
         tagged_statement != end_statement; ++tagged_statement) {
        if (tagged_statement->index >= statements_.size()) {
            statements_.resize(tagged_statement->index + 1, NULL);
            text_statements_.resize(tagged_statement->index + 1,
                                    std::string(""));
        }
        prepareStatement(tagged_statement->index,
                         tagged_statement->text);
    }
}

/// @brief Destructor
MySqlConnection::~MySqlConnection() {
    // Free up the prepared statements, ignoring errors. (What would we do
    // about them? We're destroying this object and are not really concerned
    // with errors on a database connection that is about to go away.)
    for (size_t i = 0; i < statements_.size(); ++i) {
        if (statements_[i] != NULL) {
            (void) mysql_stmt_close(statements_[i]);
            statements_[i] = NULL;
        }
    }
    statements_.clear();
    text_statements_.clear();
}

// Time conversion methods.
//
// Note that the MySQL TIMESTAMP data type (used for "expire") converts data
// from the current timezone to UTC for storage, and from UTC to the current
// timezone for retrieval.
//
// This causes no problems providing that:
// a) cltt is given in local time
// b) We let the system take care of timezone conversion when converting
//    from a time read from the database into a local time.
void
MySqlConnection::convertToDatabaseTime(const time_t input_time,
                                       MYSQL_TIME& output_time) {
    MySqlBinding::convertToDatabaseTime(input_time, output_time);
}

void
MySqlConnection::convertToDatabaseTime(const time_t cltt,
                                       const uint32_t valid_lifetime,
                                       MYSQL_TIME& expire) {
    MySqlBinding::convertToDatabaseTime(cltt, valid_lifetime, expire);
}

void
MySqlConnection::convertFromDatabaseTime(const MYSQL_TIME& expire,
                                         uint32_t valid_lifetime, time_t& cltt) {
    MySqlBinding::convertFromDatabaseTime(expire, valid_lifetime, cltt);
}

void
MySqlConnection::startTransaction() {
    // If it is nested transaction, do nothing.
    if (++transaction_ref_count_ > 1) {
        return;
    }

    DB_LOG_DEBUG(DB_DBG_TRACE_DETAIL, MYSQL_START_TRANSACTION);
    checkUnusable();
    // We create prepared statements for all other queries, but MySQL
    // don't support prepared statements for START TRANSACTION.
    int status = mysql_query(mysql_, "START TRANSACTION");
    if (status != 0) {
        isc_throw(DbOperationError, "unable to start transaction, "
                  "reason: " << mysql_error(mysql_));
    }
}

bool
MySqlConnection::isTransactionStarted() const {
    return (transaction_ref_count_ > 0);
}

void
MySqlConnection::commit() {
    if (transaction_ref_count_ <= 0) {
        isc_throw(Unexpected, "commit called for not started transaction - coding error");
    }

    // When committing nested transaction, do nothing.
    if (--transaction_ref_count_ > 0) {
        return;
    }
    DB_LOG_DEBUG(DB_DBG_TRACE_DETAIL, MYSQL_COMMIT);
    checkUnusable();
    if (mysql_commit(mysql_) != 0) {
        isc_throw(DbOperationError, "commit failed: "
                  << mysql_error(mysql_));
    }
}

void
MySqlConnection::rollback() {
    if (transaction_ref_count_ <= 0) {
        isc_throw(Unexpected, "rollback called for not started transaction - coding error");
    }

    // When rolling back nested transaction, do nothing.
    if (--transaction_ref_count_ > 0) {
        return;
    }
    DB_LOG_DEBUG(DB_DBG_TRACE_DETAIL, MYSQL_ROLLBACK);
    checkUnusable();
    if (mysql_rollback(mysql_) != 0) {
        isc_throw(DbOperationError, "rollback failed: "
                  << mysql_error(mysql_));
    }
}

template<typename T>
void
MySqlConnection::setIntParameterValue(const std::string& name, int64_t min, int64_t max, T& value) {
    string svalue;
    try {
        svalue = getParameter(name);
    } catch (...) {
        // Do nothing if the parameter is not present.
    }
    if (svalue.empty()) {
        return;
    }
    try {
        // Try to convert the value.
        auto parsed_value = boost::lexical_cast<T>(svalue);
        // Check if the value is within the specified range.
        if ((parsed_value < min) || (parsed_value > max)) {
            isc_throw(BadValue, "bad " << svalue << " value");
        }
        // Everything is fine. Return the parsed value.
        value = parsed_value;

    } catch (...) {
        // We may end up here when lexical_cast fails or when the
        // parsed value is not within the desired range. In both
        // cases let's throw the same general error.
        isc_throw(BadValue, name << " parameter (" <<
                  svalue << ") must be an integer between "
                  << min << " and " << max);
    }
}

} // namespace db
} // namespace isc
