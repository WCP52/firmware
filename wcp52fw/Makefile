all:
	@${MAKE} -f src/ASF/sam/utils/make/Makefile.sam.in all

clean:
	find -name '*.o' -delete
	find -name '*.d' -delete
	find ../scpi-parser/libscpi/src -name '*.o' -delete
	find ../scpi-parser/libscpi/src -name '*.d' -delete
	rm -f wcp52fw*.bin
	rm -f wcp52fw*.elf
	rm -f wcp52fw*.hex
	rm -f wcp52fw*.lss
	rm -f wcp52fw*.map
	rm -f wcp52fw*.sym

%: force
	@${MAKE} -f src/ASF/sam/utils/make/Makefile.sam.in $@

force: ;

