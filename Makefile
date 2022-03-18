
CC := g++
INCLUDES := src/includes
CCFLAGS := -I$(INCLUDES) -std=c++17 -lstdc++fs -Wall
CCOBJFLAGS := $(CCFLAGS) -c

OUT_PATH := out
OBJ_PATH := obj

TARGET_NAME := cclipboard

SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.cc*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
CLEAN_LIST := $(OUT_PATH)

# default rule
default: makedir all

# non-phony targets
$(TARGET): $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $(OBJ)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cc*
	$(CC) $(CCOBJFLAGS) -o $@ $<

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(OUT_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)
