include ../Makefile.inc

GEN_EXE = atomic_append bad_exclusive_open copy \
	multi_descriptors seek_io t_readv t_truncate \
	t_tee t_cp seek_append t_atomic_append imple_dup dup_and_seek

LINUX_EXE = large_file

ifeq ($(MYOWN), ON)
	CFLAGS += -DMYOWN_IPLM
endif

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}


clean :
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
