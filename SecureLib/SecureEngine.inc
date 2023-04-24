
;Secure engine 0.95 - prerelease
;WARNING - HIGH CPU USE
;trash_count - number of generated trash in one call
;trash_count > 2 - very big file size and very long execution

;--- CODE ---
;Write Secure
;Create proc _main
;Use HCall and HPush as call and push

section '.RTData' readable writeable
sdata1 db 'RCLAE-OEM', 0
sdata2 db 'CfZ01K', 0
sdata3 db 'BiosVersion', 0
sdata4 db 'Kernel32', 0
sdata5 db 'VMProtect', 0
sdata6 db 'Themida', 0
sbuf1 dd ?
sbuf2 dd ?
sbuf3 dd ?
sbuf4 dd ?
trash_count = 16

section '.runtime' code readable executable

SExit:
	call [ExitProcess]

;random gen
randseed = %t
macro randomize {
randseed = randseed * 1103515245 + 12345
randseed = (randseed / 65536) mod 0x100000000
rndnum = randseed and 0xFFFFFFFF
}

macro facke_sign {
push edx
push ecx
push ebp
push edi
db 0x64, 0x67, 0xA1, 0x30, 0x00
test eax,eax
js @f+1
call .end.sign
pop eax
add eax,7
db 0xC6
nop
ret
@@:
db 0xE9,0x00,0x00,0x00,0x00
.end.sign:
push ebx
pushad
mov ebp,0x90909090
lea eax,[ebp-0x70]
lea ebx,[ebp-0x70]
call $+5
lea eax,[ecx]
db 0xE9,0x00,0x00,0x00,0x00
}

macro save_reg{
	mov [sbuf1], eax
	mov [sbuf2], ebx
	mov [sbuf3], ecx
	mov [sbuf4], edx
}

macro load_reg{
	mov eax, [sbuf1]
	mov ebx, [sbuf2]
	mov ecx, [sbuf3]
	mov edx, [sbuf4]
}

macro gen_trash length {
repeat length
	mov [sbuf1], eax
	mov [sbuf2], ebx
	mov [sbuf3], ecx
	mov [sbuf4], edx
	
	mov eax, sdata1
	mov ebx, sdata2
	cmp eax, ebx
	je SExit
	
	mov eax, sdata4
	mov ebx, sdata5
	cmp eax, ebx
	je SExit
	
	mov eax, sdata5
	mov ebx, sdata6
	cmp eax, ebx
	je SExit
	
	mov eax, sdata1
	mov ebx, sdata5
	cmp eax, ebx
	je SExit
	
	mov eax, [sbuf1]
	mov ebx, [sbuf2]
	mov ecx, [sbuf3]
	mov edx, [sbuf4]
end repeat
}


macro call target{

	save_reg
	gen_trash trash_count
	load_reg
	
	call target
}


macro push target{
	save_reg
	gen_trash trash_count
	load_reg
	
	push target
}



proc stage1
	stage1_loop:
	mov eax,sdata1
	mov ebx,sdata3
	cmp eax, ebx
	je stage1_loop
	gen_trash trash_count
	ret
endp

proc stage2
	stage2_loop:
	mov eax,sdata3
	mov ebx,sdata5
	cmp eax, ebx
	je stage2_loop
	gen_trash trash_count
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	call _main
	ret
endp

proc stage3
	stage3_loop:
	mov eax,sdata5
	mov ebx,sdata6
	cmp eax, ebx
	je stage3_loop
	gen_trash trash_count
	ret
endp

proc stage4
	stage4_loop:
	mov eax,sdata2
	mov ebx,sdata5
	cmp eax, ebx
	je stage4_loop
	gen_trash trash_count
	ret
endp

proc stage5
	stage5_loop:
	mov eax,sdata2
	mov ebx,sdata6
	cmp eax, ebx
	je stage5_loop
	gen_trash trash_count
	ret
endp


proc SType1
	gen_trash trash_count
	call stage1
	call stage5
	call stage4
	call stage3
	call stage2
	gen_trash trash_count
	ret
endp



proc start
		ENTRY_POINT = $$
		facke_sign
		gen_trash trash_count * 2
		xstage1_loop:
		mov eax,sdata2
		mov ebx,sdata6
		cmp eax, ebx
		je xstage1_loop
		xstage2_loop:
		mov eax,sdata1
		mov ebx,sdata4
		cmp eax, ebx
		je xstage2_loop
		xstage3_loop:
		mov eax,sdata4
		mov eax,sdata1
		je xstage3_loop
		call SType1
		ret
endp


section '.vmp' readable writeable
	vmp db 'VMProtect', 0
section '.themida' readable writeable
	Themida db 'Themida Pro', 0
