#!/usr/bin/make -f

#
# Copyright (c) 2024 Man Hung-Coeng <udc577@126.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#




# Make sure that "all" is the first and the only target in this file.
all: init




LCS_ALIAS := lazy_coding
export THIRD_PARTY_DIR ?= ../3rdpary
export NO_PRIV_STUFF := $(strip $(filter-out n N no NO No 0, ${NO_PRIV_STUFF}))

-include ${THIRD_PARTY_DIR}/${LCS_ALIAS}/makefile/__ver__.mk




C_SRCS := $(shell find ./ ../3rdpary/lazy_coding/c_and_cpp/ -name "*.c" | grep -v '\.priv\.c$$')
ifeq (${NO_PRIV_STUFF},)
    C_SRCS := $(foreach i, $(filter-out %.priv.c, ${C_SRCS}), \
        $(if $(wildcard $(basename ${i}).priv.c), $(basename ${i}).priv.c, ${i}) \
    )
endif

CXX_SRCS := $(shell find ./ -name "*.cpp" -o -name "*.cc" -o -name "*.cxx" | grep -v '\.priv\.[^.]\+$$')
ifeq (${NO_PRIV_STUFF},)
    CXX_SRCS := $(foreach i, $(filter-out $(addprefix %.priv, $(suffix ${CXX_SRCS})), ${CXX_SRCS}), \
        $(if $(wildcard $(basename ${i}).priv$(suffix ${i})), $(basename ${i}).priv$(suffix ${i}), ${i}) \
    )
endif

GOAL := barcode_scanner.elf

${GOAL}: $(addsuffix .o, $(basename ${C_SRCS} ${CXX_SRCS}))

QT_INC := $(shell ls -d /usr/include/*-linux-gnu/qt* | sort -V | tail -n 1)
QT_VER := $(shell echo ${QT_INC} | sed 's/.*qt\([0-9]\+\)/\1/')
ifneq ($(shell [ ${QT_VER} -gt 5 ] && echo Y || echo ""),)
CXX_STD := c++17
endif
C_DEFINES := -U__STRICT_ANSI__
CXX_DEFINES := -DMAX_DETECT_THREADS=$(if ${MAX_DETECT_THREADS},${MAX_DETECT_THREADS},64) -DNEED_OS_SIGNALS
CXX_INCLUDES := -I/usr/include/opencv4 -I${QT_INC} -I../3rdpary/lazy_coding/c_and_cpp/native
CXX_LDFLAGS := -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_videoio \
    -lQt${QT_VER}Core -lQt${QT_VER}Gui -lZXing

-include ${THIRD_PARTY_DIR}/${LCS_ALIAS}/makefile/c_and_cpp.mk




VCS_LIST := git
VCS ?= $(word 1, ${VCS_LIST})
LCS_URL := https://github.com/FooFooDamon/lazy_coding_skills
# Format of each project item: <alias>@@<method>@@<vcs>@@<default-branch>@@<url>
# NOTES:
# 1) If the method field of an item is by-tag or by-hash,
#   then its tag or hash code needs to be set into file manually
#   after "make seeds" and before "make init".
# 2) The "partial" method only works in HTTP(S) way so far.
# 3) SVN projects are not supported yet.
THIRD_PARTY_PROJECTS := ${LCS_ALIAS}@@partial@@git@@main@@${LCS_URL} \
    #nvidia-docker-v2.12.0@@by-tag@@git@@main@@https://gitlab.com/nvidia/container-toolkit/nvidia-docker \
    #nvidia-docker-80902fe3afab@@by-hash@@git@@main@@git@gitlab.com:nvidia/container-toolkit/nvidia-docker.git \
    #rt-thread@@by-tag@@git@@master@@https://gitee.com/rtthread/rt-thread.git \
    # Beware that each line should begin with 4 spaces and end with a backslash.
CHKOUT ?= ${LCS_ALIAS}
# For a formal project, $(abspath ./__chkout__) or somewhere else might be better.
CHKOUT_CONF_DIR := ${THIRD_PARTY_DIR}

seeds:
	$(if ${Q},@printf '>>> SEEDS: Begin.\n')
	${Q}mkdir -p ${THIRD_PARTY_DIR} ${CHKOUT_CONF_DIR}
	${Q}for i in ${VCS_LIST}; \
	do \
		[ ! -s ${THIRD_PARTY_DIR}/checkout.$${i}.mk ] || continue; \
		$(if ${Q},printf "WGET\t${THIRD_PARTY_DIR}/checkout.$${i}.mk\n";) \
		wget $(if ${Q},-q) -c -O ${THIRD_PARTY_DIR}/checkout.$${i}.mk "${LCS_URL}/raw/main/makefile/checkout.$${i}.mk"; \
	done
	${Q}for i in ${THIRD_PARTY_PROJECTS}; \
	do \
		export CHKOUT_ALIAS=$$(echo "$${i}" | awk -F '@@' '{ print $$1 }'); \
		export CHKOUT_METHOD=$$(echo "$${i}" | awk -F '@@' '{ print $$2 }'); \
		export VCS_CMD=$$(echo "$${i}" | awk -F '@@' '{ print $$3 }'); \
		export CHKOUT_STEM=$$(echo "$${i}" | awk -F '@@' '{ print $$4 }'); \
		export CHKOUT_URL=$$(echo "$${i}" | awk -F '@@' '{ print $$5 }'); \
		export MKFILE=${CHKOUT_CONF_DIR}/$${CHKOUT_ALIAS}.$${VCS_CMD}.chkout.mk; \
		[ ! -e $${MKFILE} ] || continue; \
		echo "# It's better to use a relative path in a project under versioning control," > $${MKFILE}; \
		echo "# or define this variable in absolute path through command line parameter." >> $${MKFILE}; \
		echo "#export CHKOUT_PARENT_DIR := ${THIRD_PARTY_DIR}" >> $${MKFILE}; \
		echo "export CHKOUT_ALIAS := $${CHKOUT_ALIAS}" >> $${MKFILE}; \
		echo "export CHKOUT_METHOD := $${CHKOUT_METHOD}" >> $${MKFILE}; \
		echo "export CHKOUT_TAG :=" >> $${MKFILE}; \
		echo "export CHKOUT_HASH :=" >> $${MKFILE}; \
		echo "export CHKOUT_STEM := $${CHKOUT_STEM}" >> $${MKFILE}; \
		echo "export CHKOUT_URL := $${CHKOUT_URL}" >> $${MKFILE}; \
		echo "export CHKOUT_TAIL_PARAMS :=" >> $${MKFILE}; \
		if [ "$${CHKOUT_ALIAS}" = "${LCS_ALIAS}" ]; then \
			echo "export CHKOUT_PARTIAL_ITEMS := main/makefile/__ver__.mk \\" >> $${MKFILE}; \
			echo "    c43cde6b37e91c40ffa9017c4df6271335292228/makefile/c_and_cpp.mk \\" >> $${MKFILE}; \
			echo "    02defcf79b1fcc61433a9d2b36d2418044d6b484/makefile/linux_driver.mk \\" >> $${MKFILE}; \
			echo "    main/makefile/stm32_cube_ide.mk \\" >> $${MKFILE}; \
		else \
			echo "export CHKOUT_PARTIAL_ITEMS := \\" >> $${MKFILE}; \
		fi; \
		echo "    # Add more items ahead of this line if needed. \\" >> $${MKFILE}; \
		echo "    # Beware that each line should begin with 4 spaces and end with a backslash." >> $${MKFILE}; \
		echo "" >> $${MKFILE}; \
		echo "[$${MKFILE}] has been created. Edit it properly before use."; \
	done
	$(if ${Q},@printf '>>> SEEDS: Done.\n')

chkout-exec:
	${Q}for i in ${VCS_LIST}; \
	do \
		[ ! -s ${THIRD_PARTY_DIR}/checkout.$${i}.mk ] || continue; \
		echo "*** [${THIRD_PARTY_DIR}/checkout.$${i}.mk] is empty, or does not exist!" >&2; \
		echo '*** Run "make seeds" to check it out first!' >&2; \
		exit 1; \
	done
	${Q}for i in ${THIRD_PARTY_PROJECTS}; \
	do \
		export CHKOUT_ALIAS=$$(echo "$${i}" | awk -F '@@' '{ print $$1 }'); \
		export VCS_CMD=$$(echo "$${i}" | awk -F '@@' '{ print $$3 }'); \
		export MKFILE=${CHKOUT_CONF_DIR}/$${CHKOUT_ALIAS}.$${VCS_CMD}.chkout.mk; \
		if [ ! -e $${MKFILE} ]; then \
			echo "*** [$${MKFILE}] does not exist!" >&2; \
			echo '*** Run "make seeds" to create it first!' >&2; \
			exit 1; \
		fi; \
		ask_and_quit() { echo "*** Have you modified [$${MKFILE}] correctly ?!" >&2; exit 1; }; \
		$(if ${Q},printf ">>> CHKOUT: Begin checking out [$${CHKOUT_ALIAS}].\n";) \
		${MAKE} $(if ${Q},-s) checkout VCS=$${VCS_CMD} CHKOUT=$${CHKOUT_ALIAS} CHKOUT_PARENT_DIR=${THIRD_PARTY_DIR} \
			|| ask_and_quit; \
		$(if ${Q},printf ">>> CHKOUT: Done checking out [$${CHKOUT_ALIAS}].\n";) \
	done

-include ${CHKOUT_CONF_DIR}/${CHKOUT}.${VCS}.chkout.mk
-include ${THIRD_PARTY_DIR}/checkout.${VCS}.mk

pre-init:
	${Q}:

post-init:
	${Q}:

init: pre-init chkout-exec post-init




#
# If you want to re-define commands of some targets, write them here:
#

