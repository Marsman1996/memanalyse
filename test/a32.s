
a32.out:     file format elf32-i386


Disassembly of section .interp:

08048154 <.interp>:
 8048154:	2f                   	das    
 8048155:	6c                   	insb   (%dx),%es:(%edi)
 8048156:	69 62 2f 6c 64 2d 6c 	imul   $0x6c2d646c,0x2f(%edx),%esp
 804815d:	69 6e 75 78 2e 73 6f 	imul   $0x6f732e78,0x75(%esi),%ebp
 8048164:	2e 32 00             	xor    %cs:(%eax),%al

Disassembly of section .note.ABI-tag:

08048168 <.note.ABI-tag>:
 8048168:	04 00                	add    $0x0,%al
 804816a:	00 00                	add    %al,(%eax)
 804816c:	10 00                	adc    %al,(%eax)
 804816e:	00 00                	add    %al,(%eax)
 8048170:	01 00                	add    %eax,(%eax)
 8048172:	00 00                	add    %al,(%eax)
 8048174:	47                   	inc    %edi
 8048175:	4e                   	dec    %esi
 8048176:	55                   	push   %ebp
 8048177:	00 00                	add    %al,(%eax)
 8048179:	00 00                	add    %al,(%eax)
 804817b:	00 02                	add    %al,(%edx)
 804817d:	00 00                	add    %al,(%eax)
 804817f:	00 06                	add    %al,(%esi)
 8048181:	00 00                	add    %al,(%eax)
 8048183:	00 18                	add    %bl,(%eax)
 8048185:	00 00                	add    %al,(%eax)
	...

Disassembly of section .note.gnu.build-id:

08048188 <.note.gnu.build-id>:
 8048188:	04 00                	add    $0x0,%al
 804818a:	00 00                	add    %al,(%eax)
 804818c:	14 00                	adc    $0x0,%al
 804818e:	00 00                	add    %al,(%eax)
 8048190:	03 00                	add    (%eax),%eax
 8048192:	00 00                	add    %al,(%eax)
 8048194:	47                   	inc    %edi
 8048195:	4e                   	dec    %esi
 8048196:	55                   	push   %ebp
 8048197:	00 9c bc 8e 8c a7 a7 	add    %bl,-0x58587372(%esp,%edi,4)
 804819e:	73 db                	jae    804817b <_init-0x179>
 80481a0:	f6 91 95 29 cf 06    	notb   0x6cf2995(%ecx)
 80481a6:	3e                   	ds
 80481a7:	f1                   	icebp  
 80481a8:	ba                   	.byte 0xba
 80481a9:	bb                   	.byte 0xbb
 80481aa:	4a                   	dec    %edx
 80481ab:	2a                   	.byte 0x2a

Disassembly of section .gnu.hash:

080481ac <.gnu.hash>:
 80481ac:	02 00                	add    (%eax),%al
 80481ae:	00 00                	add    %al,(%eax)
 80481b0:	06                   	push   %es
 80481b1:	00 00                	add    %al,(%eax)
 80481b3:	00 01                	add    %al,(%ecx)
 80481b5:	00 00                	add    %al,(%eax)
 80481b7:	00 05 00 00 00 00    	add    %al,0x0
 80481bd:	20 00                	and    %al,(%eax)
 80481bf:	20 00                	and    %al,(%eax)
 80481c1:	00 00                	add    %al,(%eax)
 80481c3:	00 06                	add    %al,(%esi)
 80481c5:	00 00                	add    %al,(%eax)
 80481c7:	00                   	.byte 0x0
 80481c8:	ad                   	lods   %ds:(%esi),%eax
 80481c9:	4b                   	dec    %ebx
 80481ca:	e3 c0                	jecxz  804818c <_init-0x168>

Disassembly of section .dynsym:

080481cc <.dynsym>:
	...
 80481dc:	1a 00                	sbb    (%eax),%al
	...
 80481e6:	00 00                	add    %al,(%eax)
 80481e8:	12 00                	adc    (%eax),%al
 80481ea:	00 00                	add    %al,(%eax)
 80481ec:	3a 00                	cmp    (%eax),%al
	...
 80481f6:	00 00                	add    %al,(%eax)
 80481f8:	12 00                	adc    (%eax),%al
 80481fa:	00 00                	add    %al,(%eax)
 80481fc:	21 00                	and    %eax,(%eax)
	...
 8048206:	00 00                	add    %al,(%eax)
 8048208:	12 00                	adc    (%eax),%al
 804820a:	00 00                	add    %al,(%eax)
 804820c:	3f                   	aas    
	...
 8048215:	00 00                	add    %al,(%eax)
 8048217:	00 20                	add    %ah,(%eax)
 8048219:	00 00                	add    %al,(%eax)
 804821b:	00 28                	add    %ch,(%eax)
	...
 8048225:	00 00                	add    %al,(%eax)
 8048227:	00 12                	add    %dl,(%edx)
 8048229:	00 00                	add    %al,(%eax)
 804822b:	00 0b                	add    %cl,(%ebx)
 804822d:	00 00                	add    %al,(%eax)
 804822f:	00 fc                	add    %bh,%ah
 8048231:	85 04 08             	test   %eax,(%eax,%ecx,1)
 8048234:	04 00                	add    $0x0,%al
 8048236:	00 00                	add    %al,(%eax)
 8048238:	11 00                	adc    %eax,(%eax)
 804823a:	0f                   	.byte 0xf
	...

Disassembly of section .dynstr:

0804823c <.dynstr>:
 804823c:	00 6c 69 62          	add    %ch,0x62(%ecx,%ebp,2)
 8048240:	63 2e                	arpl   %bp,(%esi)
 8048242:	73 6f                	jae    80482b3 <_init-0x41>
 8048244:	2e 36 00 5f 49       	cs add %bl,%cs:%ss:0x49(%edi)
 8048249:	4f                   	dec    %edi
 804824a:	5f                   	pop    %edi
 804824b:	73 74                	jae    80482c1 <_init-0x33>
 804824d:	64 69 6e 5f 75 73 65 	imul   $0x64657375,%fs:0x5f(%esi),%ebp
 8048254:	64 
 8048255:	00 70 72             	add    %dh,0x72(%eax)
 8048258:	69 6e 74 66 00 6d 61 	imul   $0x616d0066,0x74(%esi),%ebp
 804825f:	6c                   	insb   (%dx),%es:(%edi)
 8048260:	6c                   	insb   (%dx),%es:(%edi)
 8048261:	6f                   	outsl  %ds:(%esi),(%dx)
 8048262:	63 00                	arpl   %ax,(%eax)
 8048264:	5f                   	pop    %edi
 8048265:	5f                   	pop    %edi
 8048266:	6c                   	insb   (%dx),%es:(%edi)
 8048267:	69 62 63 5f 73 74 61 	imul   $0x6174735f,0x63(%edx),%esp
 804826e:	72 74                	jb     80482e4 <_init-0x10>
 8048270:	5f                   	pop    %edi
 8048271:	6d                   	insl   (%dx),%es:(%edi)
 8048272:	61                   	popa   
 8048273:	69 6e 00 66 72 65 65 	imul   $0x65657266,0x0(%esi),%ebp
 804827a:	00 5f 5f             	add    %bl,0x5f(%edi)
 804827d:	67 6d                	insl   (%dx),%es:(%di)
 804827f:	6f                   	outsl  %ds:(%esi),(%dx)
 8048280:	6e                   	outsb  %ds:(%esi),(%dx)
 8048281:	5f                   	pop    %edi
 8048282:	73 74                	jae    80482f8 <_init+0x4>
 8048284:	61                   	popa   
 8048285:	72 74                	jb     80482fb <_init+0x7>
 8048287:	5f                   	pop    %edi
 8048288:	5f                   	pop    %edi
 8048289:	00 47 4c             	add    %al,0x4c(%edi)
 804828c:	49                   	dec    %ecx
 804828d:	42                   	inc    %edx
 804828e:	43                   	inc    %ebx
 804828f:	5f                   	pop    %edi
 8048290:	32 2e                	xor    (%esi),%ch
 8048292:	30 00                	xor    %al,(%eax)

Disassembly of section .gnu.version:

08048294 <.gnu.version>:
 8048294:	00 00                	add    %al,(%eax)
 8048296:	02 00                	add    (%eax),%al
 8048298:	02 00                	add    (%eax),%al
 804829a:	02 00                	add    (%eax),%al
 804829c:	00 00                	add    %al,(%eax)
 804829e:	02 00                	add    (%eax),%al
 80482a0:	01 00                	add    %eax,(%eax)

Disassembly of section .gnu.version_r:

080482a4 <.gnu.version_r>:
 80482a4:	01 00                	add    %eax,(%eax)
 80482a6:	01 00                	add    %eax,(%eax)
 80482a8:	01 00                	add    %eax,(%eax)
 80482aa:	00 00                	add    %al,(%eax)
 80482ac:	10 00                	adc    %al,(%eax)
 80482ae:	00 00                	add    %al,(%eax)
 80482b0:	00 00                	add    %al,(%eax)
 80482b2:	00 00                	add    %al,(%eax)
 80482b4:	10 69 69             	adc    %ch,0x69(%ecx)
 80482b7:	0d 00 00 02 00       	or     $0x20000,%eax
 80482bc:	4e                   	dec    %esi
 80482bd:	00 00                	add    %al,(%eax)
 80482bf:	00 00                	add    %al,(%eax)
 80482c1:	00 00                	add    %al,(%eax)
	...

Disassembly of section .rel.dyn:

080482c4 <.rel.dyn>:
 80482c4:	fc                   	cld    
 80482c5:	9f                   	lahf   
 80482c6:	04 08                	add    $0x8,%al
 80482c8:	06                   	push   %es
 80482c9:	04 00                	add    $0x0,%al
	...

Disassembly of section .rel.plt:

080482cc <.rel.plt>:
 80482cc:	0c a0                	or     $0xa0,%al
 80482ce:	04 08                	add    $0x8,%al
 80482d0:	07                   	pop    %es
 80482d1:	01 00                	add    %eax,(%eax)
 80482d3:	00 10                	add    %dl,(%eax)
 80482d5:	a0 04 08 07 02       	mov    0x2070804,%al
 80482da:	00 00                	add    %al,(%eax)
 80482dc:	14 a0                	adc    $0xa0,%al
 80482de:	04 08                	add    $0x8,%al
 80482e0:	07                   	pop    %es
 80482e1:	03 00                	add    (%eax),%eax
 80482e3:	00 18                	add    %bl,(%eax)
 80482e5:	a0 04 08 07 04       	mov    0x4070804,%al
 80482ea:	00 00                	add    %al,(%eax)
 80482ec:	1c a0                	sbb    $0xa0,%al
 80482ee:	04 08                	add    $0x8,%al
 80482f0:	07                   	pop    %es
 80482f1:	05                   	.byte 0x5
	...

Disassembly of section .init:

080482f4 <_init>:
 80482f4:	53                   	push   %ebx
 80482f5:	83 ec 08             	sub    $0x8,%esp
 80482f8:	e8 b3 00 00 00       	call   80483b0 <__x86.get_pc_thunk.bx>
 80482fd:	81 c3 03 1d 00 00    	add    $0x1d03,%ebx
 8048303:	8b 83 fc ff ff ff    	mov    -0x4(%ebx),%eax
 8048309:	85 c0                	test   %eax,%eax
 804830b:	74 05                	je     8048312 <_init+0x1e>
 804830d:	e8 4e 00 00 00       	call   8048360 <__gmon_start__@plt>
 8048312:	83 c4 08             	add    $0x8,%esp
 8048315:	5b                   	pop    %ebx
 8048316:	c3                   	ret    

Disassembly of section .plt:

08048320 <printf@plt-0x10>:
 8048320:	ff 35 04 a0 04 08    	pushl  0x804a004
 8048326:	ff 25 08 a0 04 08    	jmp    *0x804a008
 804832c:	00 00                	add    %al,(%eax)
	...

08048330 <printf@plt>:
 8048330:	ff 25 0c a0 04 08    	jmp    *0x804a00c
 8048336:	68 00 00 00 00       	push   $0x0
 804833b:	e9 e0 ff ff ff       	jmp    8048320 <_init+0x2c>

08048340 <free@plt>:
 8048340:	ff 25 10 a0 04 08    	jmp    *0x804a010
 8048346:	68 08 00 00 00       	push   $0x8
 804834b:	e9 d0 ff ff ff       	jmp    8048320 <_init+0x2c>

08048350 <malloc@plt>:
 8048350:	ff 25 14 a0 04 08    	jmp    *0x804a014
 8048356:	68 10 00 00 00       	push   $0x10
 804835b:	e9 c0 ff ff ff       	jmp    8048320 <_init+0x2c>

08048360 <__gmon_start__@plt>:
 8048360:	ff 25 18 a0 04 08    	jmp    *0x804a018
 8048366:	68 18 00 00 00       	push   $0x18
 804836b:	e9 b0 ff ff ff       	jmp    8048320 <_init+0x2c>

08048370 <__libc_start_main@plt>:
 8048370:	ff 25 1c a0 04 08    	jmp    *0x804a01c
 8048376:	68 20 00 00 00       	push   $0x20
 804837b:	e9 a0 ff ff ff       	jmp    8048320 <_init+0x2c>

Disassembly of section .text:

08048380 <_start>:
 8048380:	31 ed                	xor    %ebp,%ebp
 8048382:	5e                   	pop    %esi
 8048383:	89 e1                	mov    %esp,%ecx
 8048385:	83 e4 f0             	and    $0xfffffff0,%esp
 8048388:	50                   	push   %eax
 8048389:	54                   	push   %esp
 804838a:	52                   	push   %edx
 804838b:	68 e0 85 04 08       	push   $0x80485e0
 8048390:	68 70 85 04 08       	push   $0x8048570
 8048395:	51                   	push   %ecx
 8048396:	56                   	push   %esi
 8048397:	68 7d 84 04 08       	push   $0x804847d
 804839c:	e8 cf ff ff ff       	call   8048370 <__libc_start_main@plt>
 80483a1:	f4                   	hlt    
 80483a2:	66 90                	xchg   %ax,%ax
 80483a4:	66 90                	xchg   %ax,%ax
 80483a6:	66 90                	xchg   %ax,%ax
 80483a8:	66 90                	xchg   %ax,%ax
 80483aa:	66 90                	xchg   %ax,%ax
 80483ac:	66 90                	xchg   %ax,%ax
 80483ae:	66 90                	xchg   %ax,%ax

080483b0 <__x86.get_pc_thunk.bx>:
 80483b0:	8b 1c 24             	mov    (%esp),%ebx
 80483b3:	c3                   	ret    
 80483b4:	66 90                	xchg   %ax,%ax
 80483b6:	66 90                	xchg   %ax,%ax
 80483b8:	66 90                	xchg   %ax,%ax
 80483ba:	66 90                	xchg   %ax,%ax
 80483bc:	66 90                	xchg   %ax,%ax
 80483be:	66 90                	xchg   %ax,%ax

080483c0 <deregister_tm_clones>:
 80483c0:	b8 2b a0 04 08       	mov    $0x804a02b,%eax
 80483c5:	2d 28 a0 04 08       	sub    $0x804a028,%eax
 80483ca:	83 f8 06             	cmp    $0x6,%eax
 80483cd:	77 01                	ja     80483d0 <deregister_tm_clones+0x10>
 80483cf:	c3                   	ret    
 80483d0:	b8 00 00 00 00       	mov    $0x0,%eax
 80483d5:	85 c0                	test   %eax,%eax
 80483d7:	74 f6                	je     80483cf <deregister_tm_clones+0xf>
 80483d9:	55                   	push   %ebp
 80483da:	89 e5                	mov    %esp,%ebp
 80483dc:	83 ec 18             	sub    $0x18,%esp
 80483df:	c7 04 24 28 a0 04 08 	movl   $0x804a028,(%esp)
 80483e6:	ff d0                	call   *%eax
 80483e8:	c9                   	leave  
 80483e9:	c3                   	ret    
 80483ea:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi

080483f0 <register_tm_clones>:
 80483f0:	b8 28 a0 04 08       	mov    $0x804a028,%eax
 80483f5:	2d 28 a0 04 08       	sub    $0x804a028,%eax
 80483fa:	c1 f8 02             	sar    $0x2,%eax
 80483fd:	89 c2                	mov    %eax,%edx
 80483ff:	c1 ea 1f             	shr    $0x1f,%edx
 8048402:	01 d0                	add    %edx,%eax
 8048404:	d1 f8                	sar    %eax
 8048406:	75 01                	jne    8048409 <register_tm_clones+0x19>
 8048408:	c3                   	ret    
 8048409:	ba 00 00 00 00       	mov    $0x0,%edx
 804840e:	85 d2                	test   %edx,%edx
 8048410:	74 f6                	je     8048408 <register_tm_clones+0x18>
 8048412:	55                   	push   %ebp
 8048413:	89 e5                	mov    %esp,%ebp
 8048415:	83 ec 18             	sub    $0x18,%esp
 8048418:	89 44 24 04          	mov    %eax,0x4(%esp)
 804841c:	c7 04 24 28 a0 04 08 	movl   $0x804a028,(%esp)
 8048423:	ff d2                	call   *%edx
 8048425:	c9                   	leave  
 8048426:	c3                   	ret    
 8048427:	89 f6                	mov    %esi,%esi
 8048429:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi

08048430 <__do_global_dtors_aux>:
 8048430:	80 3d 28 a0 04 08 00 	cmpb   $0x0,0x804a028
 8048437:	75 13                	jne    804844c <__do_global_dtors_aux+0x1c>
 8048439:	55                   	push   %ebp
 804843a:	89 e5                	mov    %esp,%ebp
 804843c:	83 ec 08             	sub    $0x8,%esp
 804843f:	e8 7c ff ff ff       	call   80483c0 <deregister_tm_clones>
 8048444:	c6 05 28 a0 04 08 01 	movb   $0x1,0x804a028
 804844b:	c9                   	leave  
 804844c:	f3 c3                	repz ret 
 804844e:	66 90                	xchg   %ax,%ax

08048450 <frame_dummy>:
 8048450:	a1 10 9f 04 08       	mov    0x8049f10,%eax
 8048455:	85 c0                	test   %eax,%eax
 8048457:	74 1f                	je     8048478 <frame_dummy+0x28>
 8048459:	b8 00 00 00 00       	mov    $0x0,%eax
 804845e:	85 c0                	test   %eax,%eax
 8048460:	74 16                	je     8048478 <frame_dummy+0x28>
 8048462:	55                   	push   %ebp
 8048463:	89 e5                	mov    %esp,%ebp
 8048465:	83 ec 18             	sub    $0x18,%esp
 8048468:	c7 04 24 10 9f 04 08 	movl   $0x8049f10,(%esp)
 804846f:	ff d0                	call   *%eax
 8048471:	c9                   	leave  
 8048472:	e9 79 ff ff ff       	jmp    80483f0 <register_tm_clones>
 8048477:	90                   	nop
 8048478:	e9 73 ff ff ff       	jmp    80483f0 <register_tm_clones>

0804847d <main>:
 804847d:	55                   	push   %ebp
 804847e:	89 e5                	mov    %esp,%ebp
 8048480:	83 e4 f0             	and    $0xfffffff0,%esp
 8048483:	83 ec 20             	sub    $0x20,%esp
 8048486:	c7 44 24 14 0a 00 00 	movl   $0xa,0x14(%esp)
 804848d:	00 
 804848e:	8b 44 24 14          	mov    0x14(%esp),%eax
 8048492:	89 04 24             	mov    %eax,(%esp)
 8048495:	e8 b6 fe ff ff       	call   8048350 <malloc@plt>
 804849a:	89 44 24 18          	mov    %eax,0x18(%esp)
 804849e:	8b 44 24 18          	mov    0x18(%esp),%eax
 80484a2:	89 44 24 04          	mov    %eax,0x4(%esp)
 80484a6:	c7 04 24 00 86 04 08 	movl   $0x8048600,(%esp)
 80484ad:	e8 7e fe ff ff       	call   8048330 <printf@plt>
 80484b2:	8b 44 24 18          	mov    0x18(%esp),%eax
 80484b6:	c6 00 61             	movb   $0x61,(%eax)
 80484b9:	8b 44 24 18          	mov    0x18(%esp),%eax
 80484bd:	83 c0 01             	add    $0x1,%eax
 80484c0:	c6 00 62             	movb   $0x62,(%eax)
 80484c3:	8b 44 24 18          	mov    0x18(%esp),%eax
 80484c7:	83 c0 02             	add    $0x2,%eax
 80484ca:	c6 00 63             	movb   $0x63,(%eax)
 80484cd:	8b 44 24 18          	mov    0x18(%esp),%eax
 80484d1:	83 c0 03             	add    $0x3,%eax
 80484d4:	c6 00 64             	movb   $0x64,(%eax)
 80484d7:	8b 44 24 18          	mov    0x18(%esp),%eax
 80484db:	83 c0 04             	add    $0x4,%eax
 80484de:	c6 00 65             	movb   $0x65,(%eax)
 80484e1:	8b 44 24 18          	mov    0x18(%esp),%eax
 80484e5:	0f b6 00             	movzbl (%eax),%eax
 80484e8:	88 44 24 13          	mov    %al,0x13(%esp)
 80484ec:	8b 44 24 18          	mov    0x18(%esp),%eax
 80484f0:	0f b6 00             	movzbl (%eax),%eax
 80484f3:	0f be c0             	movsbl %al,%eax
 80484f6:	89 44 24 04          	mov    %eax,0x4(%esp)
 80484fa:	c7 04 24 0e 86 04 08 	movl   $0x804860e,(%esp)
 8048501:	e8 2a fe ff ff       	call   8048330 <printf@plt>
 8048506:	8b 44 24 18          	mov    0x18(%esp),%eax
 804850a:	0f b6 50 04          	movzbl 0x4(%eax),%edx
 804850e:	8b 44 24 18          	mov    0x18(%esp),%eax
 8048512:	88 10                	mov    %dl,(%eax)
 8048514:	8b 44 24 18          	mov    0x18(%esp),%eax
 8048518:	0f b6 00             	movzbl (%eax),%eax
 804851b:	0f be c0             	movsbl %al,%eax
 804851e:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048522:	c7 04 24 0e 86 04 08 	movl   $0x804860e,(%esp)
 8048529:	e8 02 fe ff ff       	call   8048330 <printf@plt>
 804852e:	8b 44 24 18          	mov    0x18(%esp),%eax
 8048532:	89 44 24 1c          	mov    %eax,0x1c(%esp)
 8048536:	8b 44 24 1c          	mov    0x1c(%esp),%eax
 804853a:	c7 00 02 00 00 00    	movl   $0x2,(%eax)
 8048540:	8b 44 24 1c          	mov    0x1c(%esp),%eax
 8048544:	8b 00                	mov    (%eax),%eax
 8048546:	8d 50 04             	lea    0x4(%eax),%edx
 8048549:	8b 44 24 1c          	mov    0x1c(%esp),%eax
 804854d:	89 10                	mov    %edx,(%eax)
 804854f:	8b 44 24 18          	mov    0x18(%esp),%eax
 8048553:	89 04 24             	mov    %eax,(%esp)
 8048556:	e8 e5 fd ff ff       	call   8048340 <free@plt>
 804855b:	b8 00 00 00 00       	mov    $0x0,%eax
 8048560:	c9                   	leave  
 8048561:	c3                   	ret    
 8048562:	66 90                	xchg   %ax,%ax
 8048564:	66 90                	xchg   %ax,%ax
 8048566:	66 90                	xchg   %ax,%ax
 8048568:	66 90                	xchg   %ax,%ax
 804856a:	66 90                	xchg   %ax,%ax
 804856c:	66 90                	xchg   %ax,%ax
 804856e:	66 90                	xchg   %ax,%ax

08048570 <__libc_csu_init>:
 8048570:	55                   	push   %ebp
 8048571:	57                   	push   %edi
 8048572:	31 ff                	xor    %edi,%edi
 8048574:	56                   	push   %esi
 8048575:	53                   	push   %ebx
 8048576:	e8 35 fe ff ff       	call   80483b0 <__x86.get_pc_thunk.bx>
 804857b:	81 c3 85 1a 00 00    	add    $0x1a85,%ebx
 8048581:	83 ec 1c             	sub    $0x1c,%esp
 8048584:	8b 6c 24 30          	mov    0x30(%esp),%ebp
 8048588:	8d b3 0c ff ff ff    	lea    -0xf4(%ebx),%esi
 804858e:	e8 61 fd ff ff       	call   80482f4 <_init>
 8048593:	8d 83 08 ff ff ff    	lea    -0xf8(%ebx),%eax
 8048599:	29 c6                	sub    %eax,%esi
 804859b:	c1 fe 02             	sar    $0x2,%esi
 804859e:	85 f6                	test   %esi,%esi
 80485a0:	74 27                	je     80485c9 <__libc_csu_init+0x59>
 80485a2:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 80485a8:	8b 44 24 38          	mov    0x38(%esp),%eax
 80485ac:	89 2c 24             	mov    %ebp,(%esp)
 80485af:	89 44 24 08          	mov    %eax,0x8(%esp)
 80485b3:	8b 44 24 34          	mov    0x34(%esp),%eax
 80485b7:	89 44 24 04          	mov    %eax,0x4(%esp)
 80485bb:	ff 94 bb 08 ff ff ff 	call   *-0xf8(%ebx,%edi,4)
 80485c2:	83 c7 01             	add    $0x1,%edi
 80485c5:	39 f7                	cmp    %esi,%edi
 80485c7:	75 df                	jne    80485a8 <__libc_csu_init+0x38>
 80485c9:	83 c4 1c             	add    $0x1c,%esp
 80485cc:	5b                   	pop    %ebx
 80485cd:	5e                   	pop    %esi
 80485ce:	5f                   	pop    %edi
 80485cf:	5d                   	pop    %ebp
 80485d0:	c3                   	ret    
 80485d1:	eb 0d                	jmp    80485e0 <__libc_csu_fini>
 80485d3:	90                   	nop
 80485d4:	90                   	nop
 80485d5:	90                   	nop
 80485d6:	90                   	nop
 80485d7:	90                   	nop
 80485d8:	90                   	nop
 80485d9:	90                   	nop
 80485da:	90                   	nop
 80485db:	90                   	nop
 80485dc:	90                   	nop
 80485dd:	90                   	nop
 80485de:	90                   	nop
 80485df:	90                   	nop

080485e0 <__libc_csu_fini>:
 80485e0:	f3 c3                	repz ret 

Disassembly of section .fini:

080485e4 <_fini>:
 80485e4:	53                   	push   %ebx
 80485e5:	83 ec 08             	sub    $0x8,%esp
 80485e8:	e8 c3 fd ff ff       	call   80483b0 <__x86.get_pc_thunk.bx>
 80485ed:	81 c3 13 1a 00 00    	add    $0x1a13,%ebx
 80485f3:	83 c4 08             	add    $0x8,%esp
 80485f6:	5b                   	pop    %ebx
 80485f7:	c3                   	ret    

Disassembly of section .rodata:

080485f8 <_fp_hw>:
 80485f8:	03 00                	add    (%eax),%eax
	...

080485fc <_IO_stdin_used>:
 80485fc:	01 00                	add    %eax,(%eax)
 80485fe:	02 00                	add    (%eax),%al
 8048600:	54                   	push   %esp
 8048601:	45                   	inc    %ebp
 8048602:	53                   	push   %ebx
 8048603:	54                   	push   %esp
 8048604:	3a 20                	cmp    (%eax),%ah
 8048606:	30 78 25             	xor    %bh,0x25(%eax)
 8048609:	30 38                	xor    %bh,(%eax)
 804860b:	78 0a                	js     8048617 <_IO_stdin_used+0x1b>
 804860d:	00                   	.byte 0x0
 804860e:	25                   	.byte 0x25
 804860f:	63 0a                	arpl   %cx,(%edx)
	...

Disassembly of section .eh_frame_hdr:

08048614 <.eh_frame_hdr>:
 8048614:	01 1b                	add    %ebx,(%ebx)
 8048616:	03 3b                	add    (%ebx),%edi
 8048618:	28 00                	sub    %al,(%eax)
 804861a:	00 00                	add    %al,(%eax)
 804861c:	04 00                	add    $0x0,%al
 804861e:	00 00                	add    %al,(%eax)
 8048620:	0c fd                	or     $0xfd,%al
 8048622:	ff                   	(bad)  
 8048623:	ff 44 00 00          	incl   0x0(%eax,%eax,1)
 8048627:	00 69 fe             	add    %ch,-0x2(%ecx)
 804862a:	ff                   	(bad)  
 804862b:	ff 68 00             	ljmp   *0x0(%eax)
 804862e:	00 00                	add    %al,(%eax)
 8048630:	5c                   	pop    %esp
 8048631:	ff                   	(bad)  
 8048632:	ff                   	(bad)  
 8048633:	ff 88 00 00 00 cc    	decl   -0x34000000(%eax)
 8048639:	ff                   	(bad)  
 804863a:	ff                   	(bad)  
 804863b:	ff c4                	inc    %esp
 804863d:	00 00                	add    %al,(%eax)
	...

Disassembly of section .eh_frame:

08048640 <__FRAME_END__-0xac>:
 8048640:	14 00                	adc    $0x0,%al
 8048642:	00 00                	add    %al,(%eax)
 8048644:	00 00                	add    %al,(%eax)
 8048646:	00 00                	add    %al,(%eax)
 8048648:	01 7a 52             	add    %edi,0x52(%edx)
 804864b:	00 01                	add    %al,(%ecx)
 804864d:	7c 08                	jl     8048657 <_IO_stdin_used+0x5b>
 804864f:	01 1b                	add    %ebx,(%ebx)
 8048651:	0c 04                	or     $0x4,%al
 8048653:	04 88                	add    $0x88,%al
 8048655:	01 00                	add    %eax,(%eax)
 8048657:	00 20                	add    %ah,(%eax)
 8048659:	00 00                	add    %al,(%eax)
 804865b:	00 1c 00             	add    %bl,(%eax,%eax,1)
 804865e:	00 00                	add    %al,(%eax)
 8048660:	c0 fc ff             	sar    $0xff,%ah
 8048663:	ff 60 00             	jmp    *0x0(%eax)
 8048666:	00 00                	add    %al,(%eax)
 8048668:	00 0e                	add    %cl,(%esi)
 804866a:	08 46 0e             	or     %al,0xe(%esi)
 804866d:	0c 4a                	or     $0x4a,%al
 804866f:	0f 0b                	ud2    
 8048671:	74 04                	je     8048677 <_IO_stdin_used+0x7b>
 8048673:	78 00                	js     8048675 <_IO_stdin_used+0x79>
 8048675:	3f                   	aas    
 8048676:	1a 3b                	sbb    (%ebx),%bh
 8048678:	2a 32                	sub    (%edx),%dh
 804867a:	24 22                	and    $0x22,%al
 804867c:	1c 00                	sbb    $0x0,%al
 804867e:	00 00                	add    %al,(%eax)
 8048680:	40                   	inc    %eax
 8048681:	00 00                	add    %al,(%eax)
 8048683:	00 f9                	add    %bh,%cl
 8048685:	fd                   	std    
 8048686:	ff                   	(bad)  
 8048687:	ff e5                	jmp    *%ebp
 8048689:	00 00                	add    %al,(%eax)
 804868b:	00 00                	add    %al,(%eax)
 804868d:	41                   	inc    %ecx
 804868e:	0e                   	push   %cs
 804868f:	08 85 02 42 0d 05    	or     %al,0x50d4202(%ebp)
 8048695:	02 e1                	add    %cl,%ah
 8048697:	c5 0c 04             	lds    (%esp,%eax,1),%ecx
 804869a:	04 00                	add    $0x0,%al
 804869c:	38 00                	cmp    %al,(%eax)
 804869e:	00 00                	add    %al,(%eax)
 80486a0:	60                   	pusha  
 80486a1:	00 00                	add    %al,(%eax)
 80486a3:	00 cc                	add    %cl,%ah
 80486a5:	fe                   	(bad)  
 80486a6:	ff                   	(bad)  
 80486a7:	ff 61 00             	jmp    *0x0(%ecx)
 80486aa:	00 00                	add    %al,(%eax)
 80486ac:	00 41 0e             	add    %al,0xe(%ecx)
 80486af:	08 85 02 41 0e 0c    	or     %al,0xc0e4102(%ebp)
 80486b5:	87 03                	xchg   %eax,(%ebx)
 80486b7:	43                   	inc    %ebx
 80486b8:	0e                   	push   %cs
 80486b9:	10 86 04 41 0e 14    	adc    %al,0x140e4104(%esi)
 80486bf:	83 05 4e 0e 30 02 48 	addl   $0x48,0x2300e4e
 80486c6:	0e                   	push   %cs
 80486c7:	14 41                	adc    $0x41,%al
 80486c9:	c3                   	ret    
 80486ca:	0e                   	push   %cs
 80486cb:	10 41 c6             	adc    %al,-0x3a(%ecx)
 80486ce:	0e                   	push   %cs
 80486cf:	0c 41                	or     $0x41,%al
 80486d1:	c7                   	(bad)  
 80486d2:	0e                   	push   %cs
 80486d3:	08 41 c5             	or     %al,-0x3b(%ecx)
 80486d6:	0e                   	push   %cs
 80486d7:	04 10                	add    $0x10,%al
 80486d9:	00 00                	add    %al,(%eax)
 80486db:	00 9c 00 00 00 00 ff 	add    %bl,-0x1000000(%eax,%eax,1)
 80486e2:	ff                   	(bad)  
 80486e3:	ff 02                	incl   (%edx)
 80486e5:	00 00                	add    %al,(%eax)
 80486e7:	00 00                	add    %al,(%eax)
 80486e9:	00 00                	add    %al,(%eax)
	...

080486ec <__FRAME_END__>:
 80486ec:	00 00                	add    %al,(%eax)
	...

Disassembly of section .init_array:

08049f08 <__frame_dummy_init_array_entry>:
 8049f08:	50                   	push   %eax
 8049f09:	84 04 08             	test   %al,(%eax,%ecx,1)

Disassembly of section .fini_array:

08049f0c <__do_global_dtors_aux_fini_array_entry>:
 8049f0c:	30                   	.byte 0x30
 8049f0d:	84 04 08             	test   %al,(%eax,%ecx,1)

Disassembly of section .jcr:

08049f10 <__JCR_END__>:
 8049f10:	00 00                	add    %al,(%eax)
	...

Disassembly of section .dynamic:

08049f14 <_DYNAMIC>:
 8049f14:	01 00                	add    %eax,(%eax)
 8049f16:	00 00                	add    %al,(%eax)
 8049f18:	01 00                	add    %eax,(%eax)
 8049f1a:	00 00                	add    %al,(%eax)
 8049f1c:	0c 00                	or     $0x0,%al
 8049f1e:	00 00                	add    %al,(%eax)
 8049f20:	f4                   	hlt    
 8049f21:	82                   	(bad)  
 8049f22:	04 08                	add    $0x8,%al
 8049f24:	0d 00 00 00 e4       	or     $0xe4000000,%eax
 8049f29:	85 04 08             	test   %eax,(%eax,%ecx,1)
 8049f2c:	19 00                	sbb    %eax,(%eax)
 8049f2e:	00 00                	add    %al,(%eax)
 8049f30:	08 9f 04 08 1b 00    	or     %bl,0x1b0804(%edi)
 8049f36:	00 00                	add    %al,(%eax)
 8049f38:	04 00                	add    $0x0,%al
 8049f3a:	00 00                	add    %al,(%eax)
 8049f3c:	1a 00                	sbb    (%eax),%al
 8049f3e:	00 00                	add    %al,(%eax)
 8049f40:	0c 9f                	or     $0x9f,%al
 8049f42:	04 08                	add    $0x8,%al
 8049f44:	1c 00                	sbb    $0x0,%al
 8049f46:	00 00                	add    %al,(%eax)
 8049f48:	04 00                	add    $0x0,%al
 8049f4a:	00 00                	add    %al,(%eax)
 8049f4c:	f5                   	cmc    
 8049f4d:	fe                   	(bad)  
 8049f4e:	ff 6f ac             	ljmp   *-0x54(%edi)
 8049f51:	81 04 08 05 00 00 00 	addl   $0x5,(%eax,%ecx,1)
 8049f58:	3c 82                	cmp    $0x82,%al
 8049f5a:	04 08                	add    $0x8,%al
 8049f5c:	06                   	push   %es
 8049f5d:	00 00                	add    %al,(%eax)
 8049f5f:	00 cc                	add    %cl,%ah
 8049f61:	81 04 08 0a 00 00 00 	addl   $0xa,(%eax,%ecx,1)
 8049f68:	58                   	pop    %eax
 8049f69:	00 00                	add    %al,(%eax)
 8049f6b:	00 0b                	add    %cl,(%ebx)
 8049f6d:	00 00                	add    %al,(%eax)
 8049f6f:	00 10                	add    %dl,(%eax)
 8049f71:	00 00                	add    %al,(%eax)
 8049f73:	00 15 00 00 00 00    	add    %dl,0x0
 8049f79:	00 00                	add    %al,(%eax)
 8049f7b:	00 03                	add    %al,(%ebx)
 8049f7d:	00 00                	add    %al,(%eax)
 8049f7f:	00 00                	add    %al,(%eax)
 8049f81:	a0 04 08 02 00       	mov    0x20804,%al
 8049f86:	00 00                	add    %al,(%eax)
 8049f88:	28 00                	sub    %al,(%eax)
 8049f8a:	00 00                	add    %al,(%eax)
 8049f8c:	14 00                	adc    $0x0,%al
 8049f8e:	00 00                	add    %al,(%eax)
 8049f90:	11 00                	adc    %eax,(%eax)
 8049f92:	00 00                	add    %al,(%eax)
 8049f94:	17                   	pop    %ss
 8049f95:	00 00                	add    %al,(%eax)
 8049f97:	00 cc                	add    %cl,%ah
 8049f99:	82                   	(bad)  
 8049f9a:	04 08                	add    $0x8,%al
 8049f9c:	11 00                	adc    %eax,(%eax)
 8049f9e:	00 00                	add    %al,(%eax)
 8049fa0:	c4 82 04 08 12 00    	les    0x120804(%edx),%eax
 8049fa6:	00 00                	add    %al,(%eax)
 8049fa8:	08 00                	or     %al,(%eax)
 8049faa:	00 00                	add    %al,(%eax)
 8049fac:	13 00                	adc    (%eax),%eax
 8049fae:	00 00                	add    %al,(%eax)
 8049fb0:	08 00                	or     %al,(%eax)
 8049fb2:	00 00                	add    %al,(%eax)
 8049fb4:	fe                   	(bad)  
 8049fb5:	ff                   	(bad)  
 8049fb6:	ff 6f a4             	ljmp   *-0x5c(%edi)
 8049fb9:	82                   	(bad)  
 8049fba:	04 08                	add    $0x8,%al
 8049fbc:	ff                   	(bad)  
 8049fbd:	ff                   	(bad)  
 8049fbe:	ff 6f 01             	ljmp   *0x1(%edi)
 8049fc1:	00 00                	add    %al,(%eax)
 8049fc3:	00 f0                	add    %dh,%al
 8049fc5:	ff                   	(bad)  
 8049fc6:	ff 6f 94             	ljmp   *-0x6c(%edi)
 8049fc9:	82                   	(bad)  
 8049fca:	04 08                	add    $0x8,%al
	...

Disassembly of section .got:

08049ffc <.got>:
 8049ffc:	00 00                	add    %al,(%eax)
	...

Disassembly of section .got.plt:

0804a000 <_GLOBAL_OFFSET_TABLE_>:
 804a000:	14 9f                	adc    $0x9f,%al
 804a002:	04 08                	add    $0x8,%al
	...
 804a00c:	36 83 04 08 46       	addl   $0x46,%ss:(%eax,%ecx,1)
 804a011:	83 04 08 56          	addl   $0x56,(%eax,%ecx,1)
 804a015:	83 04 08 66          	addl   $0x66,(%eax,%ecx,1)
 804a019:	83 04 08 76          	addl   $0x76,(%eax,%ecx,1)
 804a01d:	83                   	.byte 0x83
 804a01e:	04 08                	add    $0x8,%al

Disassembly of section .data:

0804a020 <__data_start>:
 804a020:	00 00                	add    %al,(%eax)
	...

0804a024 <__dso_handle>:
 804a024:	00 00                	add    %al,(%eax)
	...

Disassembly of section .bss:

0804a028 <__bss_start>:
 804a028:	00 00                	add    %al,(%eax)
	...

Disassembly of section .comment:

00000000 <.comment>:
   0:	47                   	inc    %edi
   1:	43                   	inc    %ebx
   2:	43                   	inc    %ebx
   3:	3a 20                	cmp    (%eax),%ah
   5:	28 55 62             	sub    %dl,0x62(%ebp)
   8:	75 6e                	jne    78 <_init-0x804827c>
   a:	74 75                	je     81 <_init-0x8048273>
   c:	20 34 2e             	and    %dh,(%esi,%ebp,1)
   f:	38 2e                	cmp    %ch,(%esi)
  11:	34 2d                	xor    $0x2d,%al
  13:	32 75 62             	xor    0x62(%ebp),%dh
  16:	75 6e                	jne    86 <_init-0x804826e>
  18:	74 75                	je     8f <_init-0x8048265>
  1a:	31 7e 31             	xor    %edi,0x31(%esi)
  1d:	34 2e                	xor    $0x2e,%al
  1f:	30 34 2e             	xor    %dh,(%esi,%ebp,1)
  22:	34 29                	xor    $0x29,%al
  24:	20 34 2e             	and    %dh,(%esi,%ebp,1)
  27:	38 2e                	cmp    %ch,(%esi)
  29:	34 00                	xor    $0x0,%al
  2b:	47                   	inc    %edi
  2c:	43                   	inc    %ebx
  2d:	43                   	inc    %ebx
  2e:	3a 20                	cmp    (%eax),%ah
  30:	28 55 62             	sub    %dl,0x62(%ebp)
  33:	75 6e                	jne    a3 <_init-0x8048251>
  35:	74 75                	je     ac <_init-0x8048248>
  37:	20 34 2e             	and    %dh,(%esi,%ebp,1)
  3a:	38 2e                	cmp    %ch,(%esi)
  3c:	34 2d                	xor    $0x2d,%al
  3e:	32 75 62             	xor    0x62(%ebp),%dh
  41:	75 6e                	jne    b1 <_init-0x8048243>
  43:	74 75                	je     ba <_init-0x804823a>
  45:	31 7e 31             	xor    %edi,0x31(%esi)
  48:	34 2e                	xor    $0x2e,%al
  4a:	30 34 2e             	xor    %dh,(%esi,%ebp,1)
  4d:	33 29                	xor    (%ecx),%ebp
  4f:	20 34 2e             	and    %dh,(%esi,%ebp,1)
  52:	38 2e                	cmp    %ch,(%esi)
  54:	34 00                	xor    $0x0,%al

Disassembly of section .debug_aranges:

00000000 <.debug_aranges>:
   0:	1c 00                	sbb    $0x0,%al
   2:	00 00                	add    %al,(%eax)
   4:	02 00                	add    (%eax),%al
   6:	00 00                	add    %al,(%eax)
   8:	00 00                	add    %al,(%eax)
   a:	04 00                	add    $0x0,%al
   c:	00 00                	add    %al,(%eax)
   e:	00 00                	add    %al,(%eax)
  10:	7d 84                	jge    ffffff96 <_end+0xf7fb5f6a>
  12:	04 08                	add    $0x8,%al
  14:	e5 00                	in     $0x0,%eax
	...

Disassembly of section .debug_info:

00000000 <.debug_info>:
   0:	ce                   	into   
   1:	00 00                	add    %al,(%eax)
   3:	00 04 00             	add    %al,(%eax,%eax,1)
   6:	00 00                	add    %al,(%eax)
   8:	00 00                	add    %al,(%eax)
   a:	04 01                	add    $0x1,%al
   c:	6c                   	insb   (%dx),%es:(%edi)
   d:	00 00                	add    %al,(%eax)
   f:	00 01                	add    %al,(%ecx)
  11:	52                   	push   %edx
  12:	00 00                	add    %al,(%eax)
  14:	00 13                	add    %dl,(%ebx)
  16:	00 00                	add    %al,(%eax)
  18:	00 7d 84             	add    %bh,-0x7c(%ebp)
  1b:	04 08                	add    $0x8,%al
  1d:	e5 00                	in     $0x0,%eax
  1f:	00 00                	add    %al,(%eax)
  21:	00 00                	add    %al,(%eax)
  23:	00 00                	add    %al,(%eax)
  25:	02 04 07             	add    (%edi,%eax,1),%al
  28:	37                   	aaa    
  29:	00 00                	add    %al,(%eax)
  2b:	00 02                	add    %al,(%edx)
  2d:	04 05                	add    $0x5,%al
  2f:	05 00 00 00 02       	add    $0x2000000,%eax
  34:	01 08                	add    %ecx,(%eax)
  36:	44                   	inc    %esp
  37:	00 00                	add    %al,(%eax)
  39:	00 02                	add    %al,(%edx)
  3b:	02 07                	add    (%edi),%al
  3d:	59                   	pop    %ecx
  3e:	00 00                	add    %al,(%eax)
  40:	00 02                	add    %al,(%edx)
  42:	04 07                	add    $0x7,%al
  44:	32 00                	xor    (%eax),%al
  46:	00 00                	add    %al,(%eax)
  48:	02 01                	add    (%ecx),%al
  4a:	06                   	push   %es
  4b:	46                   	inc    %esi
  4c:	00 00                	add    %al,(%eax)
  4e:	00 02                	add    %al,(%edx)
  50:	02 05 ad 00 00 00    	add    0xad,%al
  56:	03 04 05 69 6e 74 00 	add    0x746e69(,%eax,1),%eax
  5d:	02 08                	add    (%eax),%cl
  5f:	05 00 00 00 00       	add    $0x0,%eax
  64:	02 08                	add    (%eax),%cl
  66:	07                   	pop    %es
  67:	2d 00 00 00 02       	sub    $0x2000000,%eax
  6c:	04 07                	add    $0x7,%al
  6e:	b7 00                	mov    $0x0,%bh
  70:	00 00                	add    %al,(%eax)
  72:	04 04                	add    $0x4,%al
  74:	78 00                	js     76 <_init-0x804827e>
  76:	00 00                	add    %al,(%eax)
  78:	02 01                	add    (%ecx),%al
  7a:	06                   	push   %es
  7b:	4d                   	dec    %ebp
  7c:	00 00                	add    %al,(%eax)
  7e:	00 04 04             	add    %al,(%esp,%eax,1)
  81:	56                   	push   %esi
  82:	00 00                	add    %al,(%eax)
  84:	00 05 0e 00 00 00    	add    %al,0xe
  8a:	01 04 56             	add    %eax,(%esi,%edx,2)
  8d:	00 00                	add    %al,(%eax)
  8f:	00 7d 84             	add    %bh,-0x7c(%ebp)
  92:	04 08                	add    $0x8,%al
  94:	e5 00                	in     $0x0,%eax
  96:	00 00                	add    %al,(%eax)
  98:	01 9c 06 70 74 72 00 	add    %ebx,0x727470(%esi,%eax,1)
  9f:	01 05 72 00 00 00    	add    %eax,0x72
  a5:	02 74 18 06          	add    0x6(%eax,%ebx,1),%dh
  a9:	69 00 01 06 56 00    	imul   $0x560601,(%eax),%eax
  af:	00 00                	add    %al,(%eax)
  b1:	02 74 14 07          	add    0x7(%esp,%edx,1),%dh
  b5:	28 00                	sub    %al,(%eax)
  b7:	00 00                	add    %al,(%eax)
  b9:	01 12                	add    %edx,(%edx)
  bb:	78 00                	js     bd <_init-0x8048237>
  bd:	00 00                	add    %al,(%eax)
  bf:	02 74 13 06          	add    0x6(%ebx,%edx,1),%dh
  c3:	61                   	popa   
  c4:	72 72                	jb     138 <_init-0x80481bc>
  c6:	00 01                	add    %al,(%ecx)
  c8:	18 7f 00             	sbb    %bh,0x0(%edi)
  cb:	00 00                	add    %al,(%eax)
  cd:	02 74 1c 00          	add    0x0(%esp,%ebx,1),%dh
	...

Disassembly of section .debug_abbrev:

00000000 <.debug_abbrev>:
   0:	01 11                	add    %edx,(%ecx)
   2:	01 25 0e 13 0b 03    	add    %esp,0x30b130e
   8:	0e                   	push   %cs
   9:	1b 0e                	sbb    (%esi),%ecx
   b:	11 01                	adc    %eax,(%ecx)
   d:	12 06                	adc    (%esi),%al
   f:	10 17                	adc    %dl,(%edi)
  11:	00 00                	add    %al,(%eax)
  13:	02 24 00             	add    (%eax,%eax,1),%ah
  16:	0b 0b                	or     (%ebx),%ecx
  18:	3e 0b 03             	or     %ds:(%ebx),%eax
  1b:	0e                   	push   %cs
  1c:	00 00                	add    %al,(%eax)
  1e:	03 24 00             	add    (%eax,%eax,1),%esp
  21:	0b 0b                	or     (%ebx),%ecx
  23:	3e 0b 03             	or     %ds:(%ebx),%eax
  26:	08 00                	or     %al,(%eax)
  28:	00 04 0f             	add    %al,(%edi,%ecx,1)
  2b:	00 0b                	add    %cl,(%ebx)
  2d:	0b 49 13             	or     0x13(%ecx),%ecx
  30:	00 00                	add    %al,(%eax)
  32:	05 2e 01 3f 19       	add    $0x193f012e,%eax
  37:	03 0e                	add    (%esi),%ecx
  39:	3a 0b                	cmp    (%ebx),%cl
  3b:	3b 0b                	cmp    (%ebx),%ecx
  3d:	49                   	dec    %ecx
  3e:	13 11                	adc    (%ecx),%edx
  40:	01 12                	add    %edx,(%edx)
  42:	06                   	push   %es
  43:	40                   	inc    %eax
  44:	18 96 42 19 00 00    	sbb    %dl,0x1942(%esi)
  4a:	06                   	push   %es
  4b:	34 00                	xor    $0x0,%al
  4d:	03 08                	add    (%eax),%ecx
  4f:	3a 0b                	cmp    (%ebx),%cl
  51:	3b 0b                	cmp    (%ebx),%ecx
  53:	49                   	dec    %ecx
  54:	13 02                	adc    (%edx),%eax
  56:	18 00                	sbb    %al,(%eax)
  58:	00 07                	add    %al,(%edi)
  5a:	34 00                	xor    $0x0,%al
  5c:	03 0e                	add    (%esi),%ecx
  5e:	3a 0b                	cmp    (%ebx),%cl
  60:	3b 0b                	cmp    (%ebx),%ecx
  62:	49                   	dec    %ecx
  63:	13 02                	adc    (%edx),%eax
  65:	18 00                	sbb    %al,(%eax)
	...

Disassembly of section .debug_line:

00000000 <.debug_line>:
   0:	45                   	inc    %ebp
   1:	00 00                	add    %al,(%eax)
   3:	00 02                	add    %al,(%edx)
   5:	00 1d 00 00 00 01    	add    %bl,0x1000000
   b:	01 fb                	add    %edi,%ebx
   d:	0e                   	push   %cs
   e:	0d 00 01 01 01       	or     $0x1010100,%eax
  13:	01 00                	add    %eax,(%eax)
  15:	00 00                	add    %al,(%eax)
  17:	01 00                	add    %eax,(%eax)
  19:	00 01                	add    %al,(%ecx)
  1b:	00 74 65 73          	add    %dh,0x73(%ebp,%eiz,2)
  1f:	74 2e                	je     4f <_init-0x80482a5>
  21:	63 00                	arpl   %ax,(%eax)
  23:	00 00                	add    %al,(%eax)
  25:	00 00                	add    %al,(%eax)
  27:	00 05 02 7d 84 04    	add    %al,0x4847d02
  2d:	08 15 93 83 f3 08    	or     %dl,0x8f38393
  33:	3f                   	aas    
  34:	75 9f                	jne    ffffffd5 <_end+0xf7fb5fa9>
  36:	9f                   	lahf   
  37:	9f                   	lahf   
  38:	a1 ad 08 91 d7       	mov    0xd79108ad,%eax
  3d:	08 92 83 9f e6 bc    	or     %dl,-0x4319607d(%edx)
  43:	59                   	pop    %ecx
  44:	02 02                	add    (%edx),%al
  46:	00 01                	add    %al,(%ecx)
  48:	01                   	.byte 0x1

Disassembly of section .debug_str:

00000000 <.debug_str>:
   0:	6c                   	insb   (%dx),%es:(%edi)
   1:	6f                   	outsl  %ds:(%esi),(%dx)
   2:	6e                   	outsb  %ds:(%esi),(%dx)
   3:	67 20 6c 6f          	and    %ch,0x6f(%si)
   7:	6e                   	outsb  %ds:(%esi),(%dx)
   8:	67 20 69 6e          	and    %ch,0x6e(%bx,%di)
   c:	74 00                	je     e <_init-0x80482e6>
   e:	6d                   	insl   (%dx),%es:(%edi)
   f:	61                   	popa   
  10:	69 6e 00 2f 68 6f 6d 	imul   $0x6d6f682f,0x0(%esi),%ebp
  17:	65                   	gs
  18:	2f                   	das    
  19:	75 62                	jne    7d <_init-0x8048277>
  1b:	75 6e                	jne    8b <_init-0x8048269>
  1d:	74 75                	je     94 <_init-0x8048260>
  1f:	2f                   	das    
  20:	44                   	inc    %esp
  21:	65                   	gs
  22:	73 6b                	jae    8f <_init-0x8048265>
  24:	74 6f                	je     95 <_init-0x804825f>
  26:	70 2f                	jo     57 <_init-0x804829d>
  28:	74 65                	je     8f <_init-0x8048265>
  2a:	73 74                	jae    a0 <_init-0x8048254>
  2c:	00 6c 6f 6e          	add    %ch,0x6e(%edi,%ebp,2)
  30:	67 20 6c 6f          	and    %ch,0x6f(%si)
  34:	6e                   	outsb  %ds:(%esi),(%dx)
  35:	67 20 75 6e          	and    %dh,0x6e(%di)
  39:	73 69                	jae    a4 <_init-0x8048250>
  3b:	67 6e                	outsb  %ds:(%si),(%dx)
  3d:	65 64 20 69 6e       	gs and %ch,%fs:%gs:0x6e(%ecx)
  42:	74 00                	je     44 <_init-0x80482b0>
  44:	75 6e                	jne    b4 <_init-0x8048240>
  46:	73 69                	jae    b1 <_init-0x8048243>
  48:	67 6e                	outsb  %ds:(%si),(%dx)
  4a:	65 64 20 63 68       	gs and %ah,%fs:%gs:0x68(%ebx)
  4f:	61                   	popa   
  50:	72 00                	jb     52 <_init-0x80482a2>
  52:	74 65                	je     b9 <_init-0x804823b>
  54:	73 74                	jae    ca <_init-0x804822a>
  56:	2e 63 00             	arpl   %ax,%cs:(%eax)
  59:	73 68                	jae    c3 <_init-0x8048231>
  5b:	6f                   	outsl  %ds:(%esi),(%dx)
  5c:	72 74                	jb     d2 <_init-0x8048222>
  5e:	20 75 6e             	and    %dh,0x6e(%ebp)
  61:	73 69                	jae    cc <_init-0x8048228>
  63:	67 6e                	outsb  %ds:(%si),(%dx)
  65:	65 64 20 69 6e       	gs and %ch,%fs:%gs:0x6e(%ecx)
  6a:	74 00                	je     6c <_init-0x8048288>
  6c:	47                   	inc    %edi
  6d:	4e                   	dec    %esi
  6e:	55                   	push   %ebp
  6f:	20 43 20             	and    %al,0x20(%ebx)
  72:	34 2e                	xor    $0x2e,%al
  74:	38 2e                	cmp    %ch,(%esi)
  76:	34 20                	xor    $0x20,%al
  78:	2d 6d 33 32 20       	sub    $0x2032336d,%eax
  7d:	2d 6d 74 75 6e       	sub    $0x6e75746d,%eax
  82:	65                   	gs
  83:	3d 67 65 6e 65       	cmp    $0x656e6567,%eax
  88:	72 69                	jb     f3 <_init-0x8048201>
  8a:	63 20                	arpl   %sp,(%eax)
  8c:	2d 6d 61 72 63       	sub    $0x6372616d,%eax
  91:	68 3d 69 36 38       	push   $0x3836693d
  96:	36 20 2d 67 20 2d 66 	and    %ch,%ss:0x662d2067
  9d:	73 74                	jae    113 <_init-0x80481e1>
  9f:	61                   	popa   
  a0:	63 6b 2d             	arpl   %bp,0x2d(%ebx)
  a3:	70 72                	jo     117 <_init-0x80481dd>
  a5:	6f                   	outsl  %ds:(%esi),(%dx)
  a6:	74 65                	je     10d <_init-0x80481e7>
  a8:	63 74 6f 72          	arpl   %si,0x72(%edi,%ebp,2)
  ac:	00 73 68             	add    %dh,0x68(%ebx)
  af:	6f                   	outsl  %ds:(%esi),(%dx)
  b0:	72 74                	jb     126 <_init-0x80481ce>
  b2:	20 69 6e             	and    %ch,0x6e(%ecx)
  b5:	74 00                	je     b7 <_init-0x804823d>
  b7:	73 69                	jae    122 <_init-0x80481d2>
  b9:	7a 65                	jp     120 <_init-0x80481d4>
  bb:	74 79                	je     136 <_init-0x80481be>
  bd:	70 65                	jo     124 <_init-0x80481d0>
	...
