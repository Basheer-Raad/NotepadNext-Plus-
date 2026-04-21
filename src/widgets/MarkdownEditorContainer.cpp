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

#include "MarkdownEditorContainer.h"
#include "MarkdownPreviewWidget.h"
#include "ScintillaNext.h"

MarkdownEditorContainer::MarkdownEditorContainer(ScintillaNext *editor, QWidget *parent)
    : QStackedWidget(parent)
    , m_editor(editor)
    , m_previewWidget(nullptr)
{
    addWidget(m_editor);
    setCurrentIndex(0);
}

MarkdownEditorContainer::~MarkdownEditorContainer()
{
}

MarkdownPreviewWidget *MarkdownEditorContainer::previewWidget()
{
    ensurePreviewWidget();
    return m_previewWidget;
}

bool MarkdownEditorContainer::isPreviewMode() const
{
    return currentIndex() == 1 && m_previewWidget != nullptr;
}

void MarkdownEditorContainer::togglePreviewMode()
{
    setPreviewMode(!isPreviewMode());
}

void MarkdownEditorContainer::setPreviewMode(bool preview)
{
    if (preview == isPreviewMode()) {
        return;
    }

    if (preview) {
        ensurePreviewWidget();
        m_previewWidget->updatePreview();
        setCurrentIndex(1);
    } else {
        setCurrentIndex(0);
        m_editor->grabFocus();
    }

    emit previewModeChanged(preview);
}

void MarkdownEditorContainer::ensurePreviewWidget()
{
    if (m_previewWidget == nullptr) {
        m_previewWidget = new MarkdownPreviewWidget(m_editor, this);
        addWidget(m_previewWidget);
    }
}
