// *************************************************************************
//
// Copyright (c) 2021 Andrei Gramakov. All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// site:    https://agramakov.me
// e-mail:  mail@agramakov.me
//
// *************************************************************************

#pragma once

#include "log.h"

#define ESP_LOGE( tag, format, ... ) log_error(format, ##__VA_ARGS__)
#define ESP_LOGW( tag, format, ... ) log_warn(format, ##__VA_ARGS__)
#define ESP_LOGI( tag, format, ... ) log_info(format, ##__VA_ARGS__)
#define ESP_LOGD( tag, format, ... ) log_debug(format, ##__VA_ARGS__)
#define ESP_LOGV( tag, format, ... ) log_trace(format, ##__VA_ARGS__)
