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

#include "MarkdownPdfExporter.h"
#include "ScintillaNext.h"

#include <QPrinter>
#include <QTextDocument>
#include <QFileInfo>
#include <QDir>
#include <QUrl>

MarkdownPdfExporter::MarkdownPdfExporter(ScintillaNext *editor)
    : m_editor(editor)
    , m_pageSize(QPageSize::A4)
    , m_orientation(QPageLayout::Portrait)
    , m_margins(20, 20, 20, 20)
{
}

void MarkdownPdfExporter::setPageSize(QPageSize::PageSizeId pageSize)
{
    m_pageSize = pageSize;
}

void MarkdownPdfExporter::setOrientation(QPageLayout::Orientation orientation)
{
    m_orientation = orientation;
}

void MarkdownPdfExporter::setMargins(const QMarginsF &margins)
{
    m_margins = margins;
}

bool MarkdownPdfExporter::exportToPdf(const QString &filePath)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(m_pageSize));
    printer.setPageOrientation(m_orientation);
    printer.setPageMargins(m_margins, QPageLayout::Millimeter);

    print(&printer);
    return printer.printerState() != QPrinter::Error;
}

void MarkdownPdfExporter::print(QPrinter *printer)
{
    QTextDocument doc;

    doc.setDefaultStyleSheet(generateStylesheet());

    if (m_editor->isFile()) {
        QFileInfo fileInfo(m_editor->getFilePath());
        doc.setBaseUrl(QUrl::fromLocalFile(fileInfo.absolutePath() + QDir::separator()));
    }

    QByteArray content = m_editor->getText(m_editor->length() + 1);
    QString markdown = QString::fromUtf8(content);
    doc.setMarkdown(markdown);

    doc.print(printer);
}

QString MarkdownPdfExporter::generateStylesheet() const
{
    return QStringLiteral(R"(
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, sans-serif;
            line-height: 1.6;
            color: #24292e;
        }
        h1, h2, h3, h4, h5, h6 {
            color: #0366d6;
            margin-top: 24px;
            margin-bottom: 16px;
            font-weight: 600;
            page-break-after: avoid;
            page-break-inside: avoid;
        }
        h1 { font-size: 2em; border-bottom: 1px solid #e1e4e8; padding-bottom: 0.3em; }
        h2 { font-size: 1.5em; border-bottom: 1px solid #e1e4e8; padding-bottom: 0.3em; }
        h3 { font-size: 1.25em; }
        code, pre {
            background-color: #f6f8fa;
            font-family: 'Consolas', 'Monaco', monospace;
            border-radius: 3px;
        }
        code {
            padding: 0.2em 0.4em;
            font-size: 0.9em;
        }
        pre {
            padding: 16px;
            overflow: auto;
            page-break-inside: avoid;
        }
        pre code {
            padding: 0;
            background: none;
        }
        blockquote {
            border-left: 4px solid #0366d6;
            color: #6a737d;
            padding-left: 16px;
            margin-left: 0;
            page-break-inside: avoid;
        }
        a {
            color: #0366d6;
            text-decoration: none;
        }
        table {
            border-collapse: collapse;
            width: 100%;
            page-break-inside: avoid;
        }
        th, td {
            border: 1px solid #e1e4e8;
            padding: 8px 12px;
            text-align: left;
        }
        th {
            background-color: #f6f8fa;
        }
        hr {
            border: none;
            border-top: 1px solid #e1e4e8;
            margin: 24px 0;
        }
        img {
            max-width: 100%;
            page-break-inside: avoid;
        }
        ul, ol {
            padding-left: 2em;
        }
        li {
            margin: 0.25em 0;
        }
    )");
}
