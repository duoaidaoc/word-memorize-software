#ifndef TABLE_H
#define TABLE_H
#include "database.h"

namespace db {
/*
 * 表基类
 */
class Table {
private:
    Database &db_;                 // db传入Table的构造函数应该是已经链接状态。

public:
    Table(Database &db) : db_(db) {
        // 检查链接状态
        if(!db_.isOpen()) {
            qDebug() << "Error: Database connection is not open!";
            throw std::runtime_error("Database connection is not open!");
        }
    }
};
} // end db

#endif // TABLE_H
