#ifndef LWIP_ARCH_SYS_ARCH_H
#define LWIP_ARCH_SYS_ARCH_H

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

// Define system architecture types
typedef uint32_t sys_prot_t;
typedef uint32_t u32_t;
typedef uint8_t u8_t;

// Define system architecture functions
#define SYS_ARCH_DECL_PROTECT(lev) sys_prot_t lev
#define SYS_ARCH_PROTECT(lev) lev = save_and_disable_interrupts()
#define SYS_ARCH_UNPROTECT(lev) restore_interrupts(lev)

// Define system architecture time functions
#define SYS_ARCH_TIMEOUT 0xffffffff
#define SYS_ARCH_TIMEOUT_INFINITE 0xffffffff

// Define system architecture message functions
typedef void *sys_mbox_t;
typedef void *sys_sem_t;
typedef void *sys_mutex_t;
typedef void *sys_thread_t;

// Define system architecture message functions
#define sys_mbox_valid(mbox) ((mbox) != NULL)
#define sys_mbox_valid_val(mbox) ((mbox) != NULL)
#define sys_sem_valid(sem) ((sem) != NULL)
#define sys_sem_valid_val(sem) ((sem) != NULL)
#define sys_mutex_valid(mutex) ((mutex) != NULL)
#define sys_mutex_valid_val(mutex) ((mutex) != NULL)

// Define system architecture message functions
#define sys_mbox_set_invalid(mbox) ((mbox) = NULL)
#define sys_mbox_set_invalid_val(mbox) ((mbox) = NULL)
#define sys_sem_set_invalid(sem) ((sem) = NULL)
#define sys_sem_set_invalid_val(sem) ((sem) = NULL)
#define sys_mutex_set_invalid(mutex) ((mutex) = NULL)
#define sys_mutex_set_invalid_val(mutex) ((mutex) = NULL)

// Define system architecture message functions
#define sys_mbox_trypost(mbox, msg) (0)
#define sys_mbox_post(mbox, msg) (0)
#define sys_mbox_tryfetch(mbox, msg) (0)
#define sys_mbox_fetch(mbox, msg) (0)
#define sys_mbox_valid(mbox) ((mbox) != NULL)
#define sys_mbox_set_invalid(mbox) ((mbox) = NULL)

// Define system architecture semaphore functions
#define sys_sem_signal(sem) (0)
#define sys_sem_wait(sem) (0)
#define sys_sem_new(sem, count) (0)
#define sys_sem_free(sem) (0)

// Define system architecture mutex functions
#define sys_mutex_new(mutex) (0)
#define sys_mutex_lock(mutex) (0)
#define sys_mutex_unlock(mutex) (0)
#define sys_mutex_free(mutex) (0)

// Define system architecture thread functions
#define sys_thread_new(name, thread, arg, stacksize, prio) (0)

#endif /* LWIP_ARCH_SYS_ARCH_H */