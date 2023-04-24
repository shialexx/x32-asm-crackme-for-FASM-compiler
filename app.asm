format PE console

entry start

include 'INCLUDE/win32a.inc'
include 'SecureLib/SecureEngine.inc'

section '.data' readable writeable
	;C format
	formatStr db '%s', 0
	formatNum db '%d', 0
	
	ep db 'Enter password: ', 0
	dm db 'Done)', 10, 0
	em db 'Error(', 10, 0
	
	;vars
	passwd rd 1

	
	NULL = 0

section '.code' code readable executable
	;main code
	proc _main
		
		;printf(ep);
		mov eax, 128
		push ep
		call [printf]
	
		
		;passwd = scanf('%d', passwd);
		push passwd
		push formatNum
		call [scanf]
		
		;int eax = passwd;
		;int ebx = 118266;
		
		mov eax, [passwd]
		mov ecx, 118266
		
		mov edx, 256
		; if (eax == ebx)
		cmp eax, ecx
		jz done
		
		; else
		call error
		ret
	endp

	;reditect
	done:
		call Done
	
	;Done or error
	proc Done
		push dm
		call [printf]
		;exit()
		jmp exit
	endp
	proc error
	
		push em
		call [printf]
		;exit()
		jmp exit
		ret
	endp
	
	;exit
	exit:
		;system('pause');
		call [getch]
		
		push NULL
		;return 0; 0 = NULL
		call [ExitProcess]
		

section '.idata' import data readable
	library kernel, 'kernel32.dll',\
		msvcrt, 'msvcrt.dll'
	import kernel, \
		ExitProcess, 'ExitProcess'
	
	import msvcrt, \
		printf, 'printf', \
		getch, '_getch', \
		scanf, 'scanf'