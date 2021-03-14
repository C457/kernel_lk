#Makefile to generate mkimg

MKIMG_DIR	:= platform/tcc_shared/mkimg
IN_DIR		:= platform/tcc_shared/include
COMPILER	:= gcc

MKIMG_SRC	:= $(MKIMG_DIR)/mkimg.c

$(BUILDDIR)/mkimg: $(MKIMG_SRC)
	$(NOECHO)$(COMPILER) -o $(BUILDDIR)/mkimg $(MKIMG_SRC)

