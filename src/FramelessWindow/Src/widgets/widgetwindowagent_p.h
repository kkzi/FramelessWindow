﻿// Copyright (C) 2023-2024 Stdware Collections (https://www.github.com/stdware)
// Copyright (C) 2021-2023 wangwenx190 (Yuhang Zhao)
// SPDX-License-Identifier: Apache-2.0

#ifndef WIDGETWINDOWAGENTPRIVATE_H
#define WIDGETWINDOWAGENTPRIVATE_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the QWindowKit API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include "../core/qwkconfig.h"
#include "../core/windowagentbase_p.h"
#include "widgetwindowagent.h"

namespace QWK
{

    class WidgetWindowAgentPrivate : public WindowAgentBasePrivate
    {
        Q_DECLARE_PUBLIC(WidgetWindowAgent)
    public:
        WidgetWindowAgentPrivate();
        ~WidgetWindowAgentPrivate();

        void init();

        // Host
        QWidget *hostWidget{};

#ifdef Q_OS_MAC
        QWidget *systemButtonAreaWidget{};
        std::unique_ptr<QObject> systemButtonAreaWidgetEventFilter;
#endif

#if defined(Q_OS_WINDOWS) && QWINDOWKIT_CONFIG(ENABLE_WINDOWS_SYSTEM_BORDERS)
        void setupWindows10BorderWorkaround();
        std::unique_ptr<QObject> borderHandler;
#endif
    };

}  // namespace QWK

#endif  // WIDGETWINDOWAGENTPRIVATE_H
