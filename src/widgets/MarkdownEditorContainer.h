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

#ifndef MARKDOWNEDITORCONTAINER_H
#define MARKDOWNEDITORCONTAINER_H

#include <QStackedWidget>

class ScintillaNext;
class MarkdownPreviewWidget;

class MarkdownEditorContainer : public QStackedWidget
{
    Q_OBJECT

public:
    explicit MarkdownEditorContainer(ScintillaNext *editor, QWidget *parent = nullptr);
    ~MarkdownEditorContainer() override;

    ScintillaNext *editor() const { return m_editor; }
    MarkdownPreviewWidget *previewWidget();

    bool isPreviewMode() const;
    void togglePreviewMode();
    void setPreviewMode(bool preview);

signals:
    void previewModeChanged(bool isPreview);

private:
    void ensurePreviewWidget();

    ScintillaNext *m_editor;
    MarkdownPreviewWidget *m_previewWidget;
};

#endif // MARKDOWNEDITORCONTAINER_H
