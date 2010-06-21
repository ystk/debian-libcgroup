#ifndef _LIBCGROUP_INIT_H
#define _LIBCGROUP_INIT_H

#ifndef _LIBCGROUP_H_INSIDE
#error "Only <libcgroup.h> should be included directly."
#endif

#include <features.h>

__BEGIN_DECLS

/**
 * @defgroup group_init 1. Initialization
 * @{
 *
 * @name Initialization
 * @{
 * Application must initialize @c libcgroup using cgroup_init() before any
 * other @c libcgroup function can be called. @c libcgroup caches information
 * about mounted hierarchies (just what's mounted where, not the control groups
 * themselves) at this time. There is currently no way to refresh this cache,
 * i.e. all subsequent mounts/remounts/unmounts are not reflected in this cache
 * and @c libcgroup may produce unexpected results.
 *
 * In addition, there is no way how to clean the cache on application exit.
 *
 * @todo this is very bad... There should be at least way how to refresh the
 * cache and/or an option to refresh it automatically (does kernel provide
 * any indication, when a filesystem is mounted/unmounted?). Dtto the cleanup
 * on exit.
 */

/**
 * Initialize libcgroup. Information about mounted hierarchies are examined
 * and cached internally (just what's mounted where, not the groups themselves).
 */
int cgroup_init(void);

/**
 * Returns path where is mounted given controller. Applications should rely on
 * @c libcgroup API and not call this function directly.
 * @param controller Name of the controller
 * @param mount_point The string where the mount point location is stored.
 * 	Please note, the caller must free the mount_point.
 */
int cgroup_get_subsys_mount_point(const char *controller, char **mount_point);

/**
 * @}
 * @}
 */
__END_DECLS

#endif /* _LIBCGROUP_INIT_H */
