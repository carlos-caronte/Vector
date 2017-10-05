#pkg-config --cflags --libs check
TESTS = check_vector
check_PROGRAMS = check_vector

check_vector_SOURCES = check_vector.c $(top_builddir)/src/vector.h
check_vector_CFLAGS = -pthread
check_vector_LDADD = $(top_builddir)/src/libvector.a -lcheck_pic -lrt -lm -lsubunit
check_vector_LDFLAGS = -fprofile-arcs -ftest-coverage

