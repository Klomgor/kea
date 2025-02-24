// Copyright (C) 2016-2022 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ROTATING_FILE_H
#define ROTATING_FILE_H

#include <mutex>

/// @file rotating_file.h Defines the class, RotatingFile, which implements
/// an appending text file that rotates to a new file on a daily basis.

namespace isc {
namespace legal_log {

/// @brief RotatingFile implements an appending text file which rotates
/// to a new file on a daily basis.  The physical file name(s) are
/// determined as follows:
///
/// Forms the current file name from:
///
///    "<path>/<base_name>.<date>.txt"
///
/// where:
/// - @b path - is the pathname supplied via the constructor. The path
/// must exist and be writable by the process
///
/// - @b base_name - an arbitrary text label supplied via the constructor
///
/// - @b date - is the system date, at the time the file is opened, in local
/// time.  The format of the value is CCYYMMDD (century,year,month,day) or
/// TXXXXXXXXXXXXXXXXXXXX (XXXXXXXXXXXXXXXXXXXX is the time in seconds since
/// epoch)
///
/// Prior to each write, the system date is compared to the current file date
/// to determine if rotation is necessary (i.e. day boundary has been crossed
/// since the last write).  If so, the current file is closed, and the new
/// file is created.
///
/// Content is added to the file by passing the desired line text into
/// the method, writeln().  This method prepends the content  with the current
/// date and time and appends an EOL.
///
/// The class implements virtual methods in facilitate unit testing
/// and derives from @c BackendStore abstract class.
class RotatingFile : public BackendStore {
public:

    /// @brief Time unit type used to rotate file.
    enum class TimeUnit {
        Second,
        Day,
        Month,
        Year
    };

    /// @brief Constructor.
    ///
    /// Create a RotatingFile for the given file name without opening the file.
    /// The file will be rotated after a specific time interval has passed since
    /// the log file was created or opened. If the time unit used is day, month
    /// or year, the file will be rotated at the beginning of the respective
    /// 'count' number of days, months or years (when the write function call
    /// detects that the day, month or year has changed).
    ///
    /// @param path Directory in which file(s) will be created.
    /// @param base_name Base file name to use when creating files.
    /// @param unit The time unit used to rotate the file.
    /// @param count The number of time units used to rotate the file (0 means
    /// disabled).
    /// @param prerotate The script to be run before closing the old file.
    /// @param postrotate The script to be run after opening the new file.
    ///
    /// @throw BackendStoreError if given file name is empty.
    RotatingFile(const std::string& path,
                 const std::string& base_name,
                 const TimeUnit unit = TimeUnit::Day,
                 const uint32_t count = 1,
                 const std::string& prerotate = "",
                 const std::string& postrotate = "");

    /// @brief Destructor.
    ///
    /// The destructor does call the close method.
    virtual ~RotatingFile();

    /// @brief Opens the current file for writing.
    ///
    /// Forms the current file name if using seconds as time units:
    ///
    ///    "<path_>/<base_name_>.<TXXXXXXXXXXXXXXXXXXXX>.txt"
    ///
    /// where XXXXXXXXXXXXXXXXXXXX is the time in seconds since epoch,
    ///
    /// or if using days, months, years as time units:
    ///
    ///    "<path_>/<base_name_>.<CCYYMMDD>.txt"
    ///
    /// where CCYYMMDD is the current date in local time,
    ///
    /// and opens the file for appending. If the file does not exist
    /// it is created.  If the file is already open, the method simply
    /// returns.
    ///
    /// @throw BackendStoreError if the file cannot be opened.
    virtual void open();

    /// @brief Closes the underlying file.
    ///
    /// Method is exception safe.
    virtual void close();

    /// @brief Appends a string to the current file.
    ///
    /// Invokes rotate() and then attempts to add the new line
    /// followed by EOL to the end of the file. The content of
    /// new line will be:
    ///
    ///     "<timestamp>SP<text><EOL>"
    ///
    /// where:
    /// - @b timestamp - current local date and time as given by the
    /// strftime format "%Y-%m-%d %H:%M:%S %Z"
    ///
    /// - @b text - text supplied by the parameter
    ///
    /// - @b EOL - the character(s) generated std::endl
    ///
    /// @param addr Address or prefix (ignored).
    /// @param text String to append.
    ///
    /// @throw BackendStoreError if the write fails.
    virtual void writeln(const std::string& text, const std::string& addr);

    /// @brief Return backend type.
    ///
    /// Returns the type of the backend (e.g. "mysql", "logfile" etc.).
    ///
    /// @return Type of the backend.
    virtual std::string getType() const {
        return (std::string("logfile"));
    }

    /// @brief Returns the current file name.
    ///
    /// @return The file name.
    std::string getFileName() const {
        return (file_name_);
    }

    /// @brief Build the year-month-day string from a date.
    ///
    /// @param time_info The time info to be converted to string.
    /// @return the YYYYMMDD string.
    static std::string getYearMonthDay(const struct tm& time_info);

    /// @brief Function which updates the file name and internal timestamp from
    /// previously created file name (if it can still be used).
    ///
    /// @param time_info The current time info that should be used for the new
    /// name if previous files can not be reused.
    /// @param use_existing Flag which indicates if the name should be updated
    /// with previously created file name if the file can still be used.
    void updateFileNameAndTimestamp(struct tm& time_info, bool use_existing);

protected:

    /// @brief Open file using specified timestamp.
    ///
    /// @param time_info The current time info used to open log rotate file.
    /// @param use_existing Flag which indicates if the name should be updated
    /// with previously created file name if the file can still be used.
    virtual void openInternal(struct tm& time_info, bool use_existing);

    /// @brief Update file name with previously created file.
    ///
    /// @param time_info The current time info used to open log rotate file.
    void useExistingFiles(struct tm& time_info);

    /// @brief Rotates the file if necessary.
    ///
    /// The system date (no time component) is latter than the current file date
    /// (i.e. day boundary has been crossed), then the current physical file is
    /// closed and replaced with a newly created and open file.
    virtual void rotate();

    /// @brief Returns true if the file is open.
    ///
    /// @return True if the physical file is open, false otherwise.
    virtual bool isOpen() const;

private:
    /// @brief Appends a string to the current file.
    ///
    /// The caller must hold the mutex.
    ///
    /// @param text String to append.
    ///
    /// @throw BackendStoreError if the write fails.
    void writelnInternal(const std::string& text);

    /// @brief Directory in which the file(s) will be created.
    std::string path_;

    /// @brief Base name of the file.
    std::string base_name_;

    /// @brief The time unit used to rotate file.
    TimeUnit time_unit_;

    /// @brief The number of time units used to rotate the file.
    ///
    /// @node 0 means disable log file rotate.
    uint32_t count_;

    /// @brief Full name of the current file.
    std::string file_name_;

    /// @brief Output file stream.
    std::ofstream file_;

    /// @brief The prerotate script to call.
    std::string prerotate_;

    /// @brief The postrotate script to call.
    std::string postrotate_;

    /// @brief The timestamp used to check if the file should be rotated.
    time_t timestamp_;

    /// @brief Mutex to protect output.
    std::mutex mutex_;
};

} // namespace legal_log
} // namespace isc

#endif
