all:
	@${MAKE} -f ASF/sam/utils/make/Makefile.sam.in all

clean:
	rm *.o
	rm *.d
	rm wcp52fw*.bin
	rm wcp52fw*.elf
	rm wcp52fw*.hex
	rm wcp52fw*.lss
	rm wcp52fw*.map
	rm wcp52fw*.sym

%: force
	@${MAKE} -f ASF/sam/utils/make/Makefile.sam.in $@

force: ;

