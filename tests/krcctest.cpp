/*
 *  SPDX-FileCopyrightText: 2002-2014 David Faure <faure@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "krcc.h"

#include <QDebug>

#include <stdio.h>

void recursive_print(const KArchiveDirectory *dir, const QString &path)
{
    QStringList list = dir->entries();
    list.sort();
    for (const auto &entryName : std::as_const(list)) {
        const KArchiveEntry *entry = dir->entry(entryName);
        printf("mode=%07o %s %s %s%s %lld isdir=%d\n",
               entry->permissions(),
               entry->user().toLatin1().constData(),
               entry->group().toLatin1().constData(),
               path.toLatin1().constData(),
               entryName.toLatin1().constData(),
               entry->isFile() ? static_cast<const KArchiveFile *>(entry)->size() : 0,
               entry->isDirectory());
        if (!entry->symLinkTarget().isEmpty()) {
            printf("  (symlink to %s)\n", qPrintable(entry->symLinkTarget()));
        }
        if (entry->isDirectory()) {
            recursive_print(static_cast<const KArchiveDirectory *>(entry), path + entryName + '/');
        }
    }
}

// See karchivetest.cpp for the unittest that covers KTar.

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf(
            "\n"
            " Usage :\n"
            " ./ktartest /path/to/existing_file.tar.gz       tests listing an existing tar.gz\n");
        return 1;
    }

    KRcc rcc(argv[1]);

    if (!rcc.open(QIODevice::ReadOnly)) {
        printf("Could not open %s for reading\n", argv[1]);
        return 1;
    }

    const KArchiveDirectory *dir = rcc.directory();

    // printf("calling recursive_print\n");
    recursive_print(dir, QLatin1String(""));
    // printf("recursive_print called\n");

    rcc.close();

    return 0;
}
