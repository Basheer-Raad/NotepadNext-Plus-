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

#include "MarkdownPreviewWidget.h"
#include "ScintillaNext.h"

#include <QVBoxLayout>
#include <QPalette>
#include <QTextDocument>
#include <QShowEvent>
#include <QUrl>
#include <QFileInfo>
#include <QDir>

MarkdownPreviewWidget::MarkdownPreviewWidget(ScintillaNext *editor, QWidget *parent)
    : QWidget(parent)
    , m_editor(editor)
    , m_browser(new QTextBrowser(this))
    , m_debounceTimer(new QTimer(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_browser);

    m_browser->setOpenExternalLinks(true);
    m_browser->setReadOnly(true);

    m_debounceTimer->setSingleShot(true);
    m_debounceTimer->setInterval(150);

    connect(m_debounceTimer, &QTimer::timeout, this, &MarkdownPreviewWidget::performUpdate);
    connect(m_editor, &ScintillaNext::modified, this, &MarkdownPreviewWidget::onEditorModified);

    updatePreview();
}

MarkdownPreviewWidget::~MarkdownPreviewWidget()
{
}

void MarkdownPreviewWidget::onEditorModified()
{
    if (isVisible()) {
        m_debounceTimer->start();
    }
}

void MarkdownPreviewWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updatePreview();
}

void MarkdownPreviewWidget::performUpdate()
{
    updatePreview();
}

void MarkdownPreviewWidget::updatePreview()
{
    QByteArray content = m_editor->getText(m_editor->length() + 1);
    QString markdown = QString::fromUtf8(content);

    QString stylesheet = generateStylesheet();
    m_browser->document()->setDefaultStyleSheet(stylesheet);

    if (m_editor->isFile()) {
        QFileInfo fileInfo(m_editor->getFilePath());
        m_browser->document()->setBaseUrl(QUrl::fromLocalFile(fileInfo.absolutePath() + QDir::separator()));
    }

    m_browser->document()->setMarkdown(markdown);
}

bool MarkdownPreviewWidget::isDarkTheme() const
{
    QPalette palette = this->palette();
    QColor windowColor = palette.color(QPalette::Window);

    qreal luminance = 0.299 * windowColor.redF() +
                      0.587 * windowColor.greenF() +
                      0.114 * windowColor.blueF();

    return luminance < 0.5;
}

QString MarkdownPreviewWidget::generateStylesheet() const
{
    if (isDarkTheme()) {
        return QStringLiteral(R"(
            body {
                background-color: #1e1e1e;
                color: #d4d4d4;
                font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, sans-serif;
                line-height: 1.6;
                padding: 20px;
            }
            h1, h2, h3, h4, h5, h6 {
                color: #569cd6;
                margin-top: 24px;
                margin-bottom: 16px;
                font-weight: 600;
            }
            h1 { font-size: 2em; border-bottom: 1px solid #404040; padding-bottom: 0.3em; }
            h2 { font-size: 1.5em; border-bottom: 1px solid #404040; padding-bottom: 0.3em; }
            h3 { font-size: 1.25em; }
            code, pre {
                background-color: #2d2d2d;
                font-family: 'Consolas', 'Monaco', monospace;
                border-radius: 3px;
            }
            code {
                padding: 0.2em 0.4em;
            }
            pre {
                padding: 16px;
                overflow: auto;
            }
            pre code {
                padding: 0;
                background: none;
            }
            blockquote {
                border-left: 4px solid #569cd6;
                color: #9cdcfe;
                padding-left: 16px;
                margin-left: 0;
            }
            a {
                color: #4ec9b0;
                text-decoration: none;
            }
            a:hover {
                text-decoration: underline;
            }
            table {
                border-collapse: collapse;
                width: 100%;
            }
            th, td {
                border: 1px solid #404040;
                padding: 8px 12px;
                text-align: left;
            }
            th {
                background-color: #2d2d2d;
            }
            hr {
                border: none;
                border-top: 1px solid #404040;
                margin: 24px 0;
            }
            img {
                max-width: 100%;
            }
            ul, ol {
                padding-left: 2em;
            }
            li {
                margin: 0.25em 0;
            }
        )");
    } else {
        return QStringLiteral(R"(
            body {
                background-color: #ffffff;
                color: #24292e;
                font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, sans-serif;
                line-height: 1.6;
                padding: 20px;
            }
            h1, h2, h3, h4, h5, h6 {
                color: #0366d6;
                margin-top: 24px;
                margin-bottom: 16px;
                font-weight: 600;
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
            }
            pre {
                padding: 16px;
                overflow: auto;
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
            }
            a {
                color: #0366d6;
                text-decoration: none;
            }
            a:hover {
                text-decoration: underline;
            }
            table {
                border-collapse: collapse;
                width: 100%;
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
            }
            ul, ol {
                padding-left: 2em;
            }
            li {
                margin: 0.25em 0;
            }
        )");
    }
}
