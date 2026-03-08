# **************************************************************************** #
#                                                                              #
#    Makefile – libpnglode                                                     #
#                                                                              #
#    PNG encoder / decoder library                                             #
#                                                                              #
# **************************************************************************** #

NAME        := libpnglode.a

# ── Directories ──────────────────────────────────────────────────────────── #

BUILD_DIR   := build
OBJ_DIR     := $(BUILD_DIR)/obj
DEP_DIR     := $(BUILD_DIR)/deps
LIB_DIR     := $(BUILD_DIR)/lib

# ── Toolchain ────────────────────────────────────────────────────────────── #

CC          := cc
AR          := gcc-ar
ARFLAGS     := rcs

CFLAGS      := -std=c99 -O3 -march=native -fno-math-errno -flto \
               -DNDEBUG -pipe \
               -D_POSIX_C_SOURCE=200809L \
               -Wall -Wextra -Werror

INCLUDES    := -I .

# ── Sources ──────────────────────────────────────────────────────────────── #

SRCS        := $(wildcard ./*.c)
OBJS        := $(SRCS:./%.c=$(OBJ_DIR)/%.o)
DEPS        := $(OBJS:$(OBJ_DIR)/%.o=$(DEP_DIR)/%.d)

# ── Phony targets ────────────────────────────────────────────────────────── #

.PHONY: all clean fclean re

all: $(LIB_DIR)/$(NAME)

# ── Directory creation ───────────────────────────────────────────────────── #

$(OBJ_DIR) $(DEP_DIR) $(LIB_DIR):
	@mkdir -p $@

# ── Compile ──────────────────────────────────────────────────────────────── #

$(OBJ_DIR)/%.o: ./%.c | $(OBJ_DIR) $(DEP_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -MF $(DEP_DIR)/$*.d -c $< -o $@

# ── Archive ──────────────────────────────────────────────────────────────── #

$(LIB_DIR)/$(NAME): $(OBJS) | $(LIB_DIR)
	@printf "\033[35m[png_writer]\033[0m libpnglode.a\n"
	@$(AR) $(ARFLAGS) $@ $?

# ── House-keeping ────────────────────────────────────────────────────────── #

clean:
	@rm -rf $(BUILD_DIR)

fclean: clean

re: fclean all

# ── Auto-dependency inclusion ────────────────────────────────────────────── #

-include $(wildcard $(DEP_DIR)/*.d)
