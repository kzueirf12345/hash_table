.PHONY: all build clean rebuild \
		logger_build logger_clean logger_rebuild \
		fist_build fist_clean fist_rebuild \
		clean_all clean_log clean_out clean_obj clean_deps clean_txt clean_bin


PROJECT_NAME = hash_table

BUILD_DIR = ./build
SRC_DIR = ./src
COMPILER ?= gcc

DEBUG_ ?= 1
USE_AVX2 ?= 1

ifeq ($(origin FLAGS), undefined)


ifeq ($(COMPILER),gcc)

FLAGS =	-Wall -Wextra -Waggressive-loop-optimizations \
		-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
		-Wconversion -Wempty-body -Wfloat-equal \
		-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
		-Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self \
		-Wredundant-decls -Wshadow -Wsign-conversion \
		-Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods \
		-Wsuggest-final-types -Wswitch-default -Wswitch-enum -Wsync-nand \
		-Wundef -Wunreachable-code -Wunused -Wvariadic-macros \
		-Wno-missing-field-initializers -Wno-narrowing -Wno-varargs \
		-Wstack-protector -fcheck-new -fstack-protector -fstrict-overflow \
		-fno-omit-frame-pointer -Wlarger-than=8192000 -Wstack-usage=8192000 -pie \
		-fPIE -Werror=vla -flto-odr-type-merging\

SANITIZER = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,$\
		integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,$\
		shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

else

FLAGS = -Wall -Wextra \
        -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
        -Wconversion -Wempty-body -Wfloat-equal \
        -Wformat-security -Wformat-signedness -Wformat=2 -Winline \
        -Wpointer-arith -Winit-self \
        -Wredundant-decls -Wshadow \
        -Wstrict-overflow=2 -Wswitch-default -Wswitch-enum \
        -Wundef -Wunreachable-code -Wunused -Wvariadic-macros \
        -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs \
        -fstack-protector -fstrict-overflow \
        -fno-omit-frame-pointer \
        -fPIE -Werror=vla -flto=thin

SANITIZER = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,\
            integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,shift,\
            signed-integer-overflow,undefined,unreachable,vptr

endif

DEBUG_FLAGS = -D _DEBUG  -ggdb -g3 -D_FORTIFY_SOURCES=3 $(SANITIZER)
RELEASE_FLAGS = -DNDEBUG

ifneq ($(DEBUG_),0)
OPTIMIZE_LVL ?= -Og
FLAGS += $(DEBUG_FLAGS) 
else
OPTIMIZE_LVL ?= -O2
FLAGS += $(RELEASE_FLAGS)
endif

FLAGS += $(OPTIMIZE_LVL)

ifneq ($(USE_AVX2),0)
FLAGS += -mavx -mavx2 -march=native -fopenmp
endif

endif

FLAGS += $(ADD_FLAGS)

LIBS = -lm -L./libs/logger -llogger -L./libs/list_on_array -lfist


DIRS = utils flags smash_map smash_map/funcs smash_map/verification smash_map/dumb test
BUILD_DIRS = $(DIRS:%=$(BUILD_DIR)/%)

SOURCES = main.c utils/utils.c flags/flags.c smash_map/funcs/funcs.c \
		  smash_map/verification/verification.c smash_map/dumb/dumb.c test/test.c test/utils.c

SOURCES_REL_PATH = $(SOURCES:%=$(SRC_DIR)/%)
OBJECTS_REL_PATH = $(SOURCES:%.c=$(BUILD_DIR)/%.o)
DEPS_REL_PATH = $(OBJECTS_REL_PATH:%.o=%.d)


all: build start

start:
	./$(PROJECT_NAME).out $(OPTS)

build: $(PROJECT_NAME).out

rebuild: clean_all build


$(PROJECT_NAME).out: $(OBJECTS_REL_PATH)
	@$(COMPILER) $(FLAGS) -o $@ $^  $(LIBS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c | ./$(BUILD_DIR)/ $(BUILD_DIRS) logger_build fist_build
	@$(COMPILER) $(FLAGS) -I$(SRC_DIR) -I./libs -I./assets -c -MMD -MP $< -o $@

-include $(DEPS_REL_PATH)

$(BUILD_DIRS):
	mkdir $@
./$(BUILD_DIR)/:
	mkdir $@


logger_rebuild: logger_build logger_clean

logger_build:
	@make ADD_FLAGS="$(ADD_FLAGS)" FLAGS="$(FLAGS)" DEBUG_=$(DEBUG_) build -C ./libs/logger

logger_clean:
	make ADD_FLAGS="$(ADD_FLAGS)" clean -C ./libs/logger

fist_rebuild: fist_build fist_clean

fist_build:
	@make ADD_FLAGS="$(ADD_FLAGS)" FLAGS="$(FLAGS)" DEBUG_=$(DEBUG_) build -C ./libs/list_on_array

fist_clean:
	make ADD_FLAGS="$(ADD_FLAGS)" clean -C ./libs/list_on_array



clean_all: clean_obj clean_deps clean_out logger_clean fist_clean

clean: clean_obj clean_deps clean_out

clean_log:
	rm -rf ./log/*

clean_out:
	rm -rf ./*.out

clean_obj:
	rm -rf ./$(OBJECTS_REL_PATH)

clean_deps:
	rm -rf ./$(DEPS_REL_PATH)

clean_txt:
	rm -rf ./*.txt

clean_bin:
	rm -rf ./*.bin

clean_gcda:
	sudo find ./ -type f -name "*.gcda" -exec rm -f {} \;

# make OPTS="-i 3 ./assets/onegin.txt ./assets/onegin_out.txt ./assets/hobbit.txt ./assets/hobbit_out.txt ./assets/wap.txt ./assets/wap_out.txt" DEBUG_=0