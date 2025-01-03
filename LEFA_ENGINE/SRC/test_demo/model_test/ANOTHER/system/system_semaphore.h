/* Lips of Suna
 * Copyright© 2007-2012 Lips of Suna development team.
 *
 * Lips of Suna is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Lips of Suna is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Lips of Suna. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SYSTEM_SEMAPHORE_H__
#define __SYSTEM_SEMAPHORE_H__

#include"system_memory.h"

typedef struct _LISysSemaphore LISysSemaphore;

LIAPICALL(LISysSemaphore*, lisys_semaphore_new, ());

LIAPICALL(void, lisys_semaphore_free, (LISysSemaphore* self));

LIAPICALL(void, lisys_semaphore_signal, (LISysSemaphore* self));

LIAPICALL(void, lisys_semaphore_wait, (LISysSemaphore* self));

#endif
