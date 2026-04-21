/*
 * This file is part of Notepad Next.
 * Copyright 2024 Justin Dailey
 *
 * Notepad Next is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Notepad Next is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Notepad Next.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MARKDOWNPDFEXPORTER_H
#define MARKDOWNPDFEXPORTER_H

#include <QString>
#include <QPageSize>
#include <QPageLayout>
#include <QMarginsF>

class ScintillaNext;
class QPrinter;

class MarkdownPdfExporter
{
public:
    explicit MarkdownPdfExporter(ScintillaNext *editor);

    void setPageSize(QPageSize::PageSizeId pageSize);
    void setOrientation(QPageLayout::Orientation orientation);
    void setMargins(const QMarginsF &margins);

    bool exportToPdf(const QString &filePath);
    void print(QPrinter *printer);

private:
    QString generateStylesheet() const;

    ScintillaNext *m_editor;
    QPageSize::PageSizeId m_pageSize;
    QPageLayout::Orientation m_orientation;
    QMarginsF m_margins;
};

#endif // MARKDOWNPDFEXPORTER_H
