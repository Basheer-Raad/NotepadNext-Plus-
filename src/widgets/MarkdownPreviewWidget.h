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

#ifndef MARKDOWNPREVIEWWIDGET_H
#define MARKDOWNPREVIEWWIDGET_H

#include <QWidget>
#include <QTextBrowser>
#include <QTimer>

class ScintillaNext;

class MarkdownPreviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MarkdownPreviewWidget(ScintillaNext *editor, QWidget *parent = nullptr);
    ~MarkdownPreviewWidget() override;

    void updatePreview();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void onEditorModified();
    void performUpdate();

private:
    QString generateStylesheet() const;
    bool isDarkTheme() const;

    ScintillaNext *m_editor;
    QTextBrowser *m_browser;
    QTimer *m_debounceTimer;
};

#endif // MARKDOWNPREVIEWWIDGET_H
