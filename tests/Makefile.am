INCLUDES = -I$(top_srcdir)/include
LDADD = $(top_builddir)/src/.libs/libcgroup.la

# compile the tests, but do not install them
noinst_PROGRAMS = libcgrouptest01 libcg_ba setuid pathtest walk_test read_stats walk_task get_controller get_mount_point proctest get_all_controller get_variable_names test_named_hierarchy

libcgrouptest01_SOURCES=libcgrouptest01.c test_functions.c libcgrouptest.h
libcg_ba_SOURCES=libcg_ba.cpp
setuid_SOURCES=setuid.c
pathtest_SOURCES=pathtest.c
walk_test_SOURCES=walk_test.c
read_stats_SOURCES=read_stats.c
walk_task_SOURCES=walk_task.c
get_controller_SOURCES=get_controller.c
get_mount_point_SOURCES=get_mount_point.c
proctest_SOURCES=proctest.c
get_all_controller_SOURCES=get_all_controller.c
get_variable_names_SOURCES=get_variable_names.c
test_named_hierarchy_SOURCES=test_named_hierarchy.c

EXTRA_DIST = pathtest.sh runlibcgrouptest.sh

TESTS = runlibcgrouptest.sh
