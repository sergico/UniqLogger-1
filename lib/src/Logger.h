/********************************************************************************
 *   Copyright (C) 2010-2018 by NetResults S.r.l. ( http://www.netresults.it )  *
 *   Author(s):                                                                 *
 *              Francesco Lamonica      <f.lamonica@netresults.it>              *
 ********************************************************************************/

#ifndef UNQL_LOGGER_INCS
#define UNQL_LOGGER_INCS

#include <QStringList>
#include <QObject>
#include <QMutex>
#include <QMap>
#include <QVariant>

#include "LogMessage.h"
#include "bufferofstrings.h"

#define UNQL_ERRMSG_SIZE 5000


class UniqLogger;
class Logger;
class LogWriter;

class ULOG_LIB_API Logger: public QObject
{
    Q_OBJECT

    int m_logVerbosityAcceptedLevel, m_logVerbosityDefaultLevel;
    bool m_printToStdOut, m_printToQDebug;
    QString m_moduleName, m_errorPrefix, m_timeStampFormat, m_spacingString;
    QMap<QThread*, BufferOfStrings> m_bufferedStreamMessages;
    QChar m_startEncasingChar, m_endEncasingChar;

    mutable QList<LogWriter*> m_logDeviceList;
    QMap<QString, bool>* m_varMonitorMap;
    QMutex *muxMonitorVar;
    QMutex muxMessages, muxDevices;

    void dispatchMessage(const LogMessage &);
    void dispatchBufferedMessages();
    void priv_log(int loglevel, const QString& message);
    UNQL::LogMessagePriorityType selectCorrectLogLevel(int chosenPriority) const;

    //we define UniqLogger to be friend so it can:
    //* set the map and its mutex
    //* access ctor and addLogDevice() and removeLogDevice()
    friend class UniqLogger;

    void printAlsoToConsoleIfRequired(const QString &message);
    
signals:
    void writersToDelete(const QList<LogWriter*>);

protected:
    Logger();
    int addLogDevice    ( LogWriter * );
    int removeLogDevice ( LogWriter * );

public:
    virtual ~Logger();

    //SETTERS
    void setVerbosityAcceptedLevel  ( const int&        );
    void setVerbosityDefaultLevel   ( const int&        );
    void setModuleName              ( const QString&    );
    void setTimeStampFormat         ( const QString&    );
    void setSpacingString           ( const QString &   );
    void setEncasingChars           ( const QChar&, const QChar& );
    void printToStdOut(bool enable);
    void printToQDebug(bool enable);

    //GETTERS
    int getVerbosityLevel() const;
    int getVerbosityAcceptedLevel() const;
    int getVerbosityDefaultLevel() const;
    QString getModuleName() const       { return m_moduleName;      }
    QString getTStampFmtString() const  { return m_timeStampFormat; }
    QString getSpacingString() const    { return m_spacingString;   }
    bool printToStdOut() const          { return m_printToStdOut;   }
    bool printToQDebug() const          { return m_printToQDebug;   }

    //LOGGING
    void log( int, const char*, ... );
    void log( const char*, ...      );
    void flush();
    void monitor( const QVariant &data, const QString &key, const QString &desc="");

    Logger& operator<< ( const UNQL::LogMessagePriorityType& d      );
    Logger& operator<< ( const UNQL::LogStreamManipType& d          );
    Logger& operator<< ( const QStringList& sl          );
    Logger& operator<< ( const QList<int>& vl           );
    Logger& operator<< ( const QMap<int, QList<int> >&  );
    Logger& operator<< ( const QVariant& v              );
    Logger& operator<< ( unsigned long                  );

    /* VS2010 does not allow a default template argument for non-template classes
    // Use SFINAE with second default template param to validate numeric types
    template <typename T, typename std::enable_if< std::is_arithmetic<T>::value, int>::type = 0>
    Logger& operator<<(T x){
        return (*this << QString::number(x));
    };
    */

};

#endif

