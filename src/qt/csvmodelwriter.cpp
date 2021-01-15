// Copyright (c) 2011-2013 The Bitcoin developers
// Copyright (c) 2017-2018 The PIVX developers
// Copyright (c) 2018-2021 The Dequant developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "csvmodelwriter.h"

#include <QAbstractItemModel>
#include <QFile>
#include <QTextStream>

CSVModelWriter::CSVModelWriter(const QString& filename, QObject* parent) : QObject(parent),
                                                                           filename(filename), model(0)
{
}

void CSVModelWriter::setModel(const QAbstractItemModel* model)
{
    this->model = model;
}

void CSVModelWriter::addColumn(const QString& title, int column, int role)
{
    Column col;
    col.title = title;
    col.column = column;
    col.role = role;

    coludeq.append(col);
}

static void writeValue(QTextStream& f, const QString& value)
{
    QString escaped = value;
    escaped.replace('"', "\"\"");
    f << "\"" << escaped << "\"";
}

static void writeSep(QTextStream& f)
{
    f << ",";
}

static void writeNewline(QTextStream& f)
{
    f << "\n";
}

bool CSVModelWriter::write()
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QTextStream out(&file);

    int numRows = 0;
    if (model) {
        numRows = model->rowCount();
    }

    // Header row
    for (int i = 0; i < coludeq.size(); ++i) {
        if (i != 0) {
            writeSep(out);
        }
        writeValue(out, coludeq[i].title);
    }
    writeNewline(out);

    // Data rows
    for (int j = 0; j < numRows; ++j) {
        for (int i = 0; i < coludeq.size(); ++i) {
            if (i != 0) {
                writeSep(out);
            }
            QVariant data = model->index(j, coludeq[i].column).data(coludeq[i].role);
            writeValue(out, data.toString());
        }
        writeNewline(out);
    }

    file.close();

    return file.error() == QFile::NoError;
}
