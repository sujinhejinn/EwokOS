XSERVERD_OBJS = $(ROOT_DIR)/sbin/dev/xserverd/xserverd.o

XSERVERD = $(TARGET_DIR)/$(ROOT_DIR)/sbin/dev/xserverd

PROGS += $(XSERVERD)
CLEAN += $(XSERVERD_OBJS)

$(XSERVERD): $(XSERVERD_OBJS) $(LIB_OBJS)
	$(LD) -Ttext=100 $(XSERVERD_OBJS) -o $(XSERVERD) $(LDFLAGS) -lgraph -lsconf -lewokc -lc
