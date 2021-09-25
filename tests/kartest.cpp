/* This file is part of the KDE project
   SPDX-FileCopyrightText: 2002-2019 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kar.h"

#include <QDebug>

#include <stdio.h>

void recursive_print(const KArchiveDirectory *dir, const QString &path)
{
    QStringList list = dir->entries();
    list.sort();
    for (const auto &entryName : std::as_const(list)) {
        const KArchiveEntry *entry = dir->entry(entryName);
        printf("mode=%7o path=%s type=%s size=%lld\n",
               entry->permissions(),
               qPrintable(path + entryName),
               entry->isFile() ? "file" : "dir",
               entry->isFile() ? static_cast<const KArchiveFile *>(entry)->size() : 0);
        if (!entry->symLinkTarget().isEmpty()) {
            printf("  (symlink to %s)\n", qPrintable(entry->symLinkTarget()));
        }
        if (entry->isDirectory()) {
            recursive_print(static_cast<const KArchiveDirectory *>(entry), path + entryName + '/');
        }
    }
}

// See karchivetest.cpp for the unittest that covers KAr.

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf(
            "\n"
            " Usage :\n"
            " ./kartest /path/to/existing_file.a       tests listing an existing archive\n");
        return 1;
    }

    KAr archive(argv[1]);

    if (!archive.open(QIODevice::ReadOnly)) {
        printf("Could not open %s for reading\n", argv[1]);
        return 1;
    }

    const KArchiveDirectory *dir = archive.directory();

    // printf("calling recursive_print\n");
    recursive_print(dir, QLatin1String(""));
    // printf("recursive_print called\n");

    archive.close();

    return 0;
}
