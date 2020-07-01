/* in file addargs.c */
extern	status	addargs(pid32, int32, int32[], int32,char *, void *);

/* in file ascdate.c */
extern	status	ascdate(uint32, char *);

/* in file bufinit.c */
extern	status	bufinit(void);

/* in file chprio.c */
extern	pri16	chprio(pid32, pri16);

/* in file clkupdate.S */
extern	uint32	clkcount(void);

/* in file clkhandler.c */
extern	interrupt clkhandler(void);

/* in file clkinit.c */
extern	void	clkinit(void);

/* in file clkdisp.S */
extern	void	clkdisp(void);

/* in file close.c */
extern	syscall	close(did32);

/* in file control.c */
extern	syscall	control(did32, int32, int32, int32);

/* in file create.c */
extern	pid32	create(void *, uint32, pri16, char *, uint32, ...);

/* in file ctxsw.S */
extern	void	ctxsw(void *, void *);

/* in file dhcp.c */
extern	uint32	getlocalip(void);

/* in file dns.c */
extern	uint32	dnslookup(char *);

/* in file dot2ip.c */
extern	uint32	dot2ip(char *, uint32 *);

/* in file getticks.c */
extern	uint64	getticks(void);

/* in file intutils.S */
extern	intmask	disable(void);

/* in file intutils.S */
extern	void	enable(void);

/* in file exit.c */
extern	void	exit(void);

/* in file kprintf.c */
extern int console_init(void);

/* in file early_imr.c */
extern int remove_irm_protections(void);

/* in file evec.c */
extern	int32	initevec(void);
extern	int32	set_evec(uint32, uint32);
extern	void	trap(int32);

/* in file exception.c */
extern  void exception(int32, int32*);

/* in file freebuf.c */
extern	syscall	freebuf(char *);

/* in file freemem.c */
extern	syscall	freemem(char *, uint32);

/* in file getbuf.c */
extern	char	*getbuf(bpid32);

/* in file getc.c */
extern	syscall	getc(did32);

/* in file getitem.c */
extern	pid32	getfirst(qid16);
extern	pid32	getlast(qid16);
extern	pid32	getitem(pid32);

/* in file getmem.c */
extern	char	*getmem(uint32);

/* in file getpid.c */
extern	pid32	getpid(void);

/* in file getprio.c */
extern	syscall	getprio(pid32);

/* in file getstk.c */
extern	char	*getstk(uint32);

/* in file gettime.c */
extern	status	gettime(uint32 *);

/* in file getutime.c */
extern	status	getutime(uint32 *);

/* in file halt.S */
extern	void	halt(void);

/* in file init.c */
extern	syscall	init(did32);

/* in file initialize.c */
extern	int32	sizmem(void);

/* in file insert.c */
extern	status	insert(pid32, qid16, int32);

/* in file insertd.c */
extern	status	insertd(pid32, qid16, int32);

/* in file intr.S */
extern	uint16	getirmask(void);

/* in file ioerr.c */
extern	devcall	ioerr(void);

/* in file ionull.c */
extern	devcall	ionull(void);

/* in file kill.c */
extern	syscall	kill(pid32);

/* in file lexan.c */
extern	int32	lexan(char *, int32, char *, int32 *, int32 [], int32 []);

/* in file lpgetc.c */
extern	devcall	lpgetc(struct dentry *);

/* in file lpinit.c */
extern	devcall	lpinit(struct dentry *);

/* in file lpopen.c */
extern	devcall	lpopen(struct dentry *, char *, char *);

/* in file lpputc.c */
extern	devcall	lpputc(struct dentry *, char);

/* in file lpread.c */
extern	devcall	lpread(struct dentry *, char *, int32);

/* in file lpwrite.c */
extern	devcall	lpwrite(struct dentry *, char *, int32);

/* in file mark.c */
extern	void	markinit(void);
extern	status	mark(int32 *);

/* in file memcpy.c */
extern	void	*memcpy(void *, const void *, int32);

/* in file memcpy.c */
extern	int32	*memcmp(void *, const void *, int32);

/* in file memset.c */
extern  void    *memset(void *, const int, int32);

/* in file mkbufpool.c */
extern	bpid32	mkbufpool(int32, int32);

/* in file mount.c */
extern	syscall	mount(char *, char *, did32);
extern	int32	namlen(char *, int32);

/* in file naminit.c */
extern	status	naminit(void);

/* in file nammap.c */
extern	devcall	nammap(char *, char[], did32);
extern	did32	namrepl(char *, char[]);
extern	status	namcpy(char *, char *, int32);

/* in file namopen.c */
extern	devcall	namopen(struct dentry *, char *, char *);

/* in file newqueue.c */
extern	qid16	newqueue(void);

/* in file open.c */
extern	syscall	open(did32, char *, char *);

/* in file panic.c */
extern	void	panic(char *);

/* in file pci.c */
extern	int32	pci_init(void);

/* in file platinit.c */
extern	void	platinit(void);

/* in file ptclear.c */
extern	void	_ptclear(struct ptentry *, uint16, int32 (*)(int32));

/* in file ptcount.c */
extern	int32	ptcount(int32);

/* in file ptcreate.c */
extern	syscall	ptcreate(int32);

/* in file ptdelete.c */
extern	syscall	ptdelete(int32, int32 (*)(int32));

/* in file ptinit.c */
extern	syscall	ptinit(int32);

/* in file ptrecv.c */
extern	uint32	ptrecv(int32);

/* in file ptreset.c */
extern	syscall	ptreset(int32, int32 (*)(int32));

/* in file ptsend.c */
extern	syscall	ptsend(int32, umsg32);

/* in file putc.c */
extern	syscall	putc(did32, char);

/* in file quark_irq.c */
extern	int32	quark_irq_routing(void);

/* in file queue.c */
extern	pid32	enqueue(pid32, qid16);
extern	pid32	dequeue(qid16);

/* in file ramclose.c */
extern	devcall	ramclose(struct dentry *);

/* in file raminit.c */
extern	devcall	raminit(struct dentry *);

/* in file ramopen.c */
extern	devcall	ramopen(struct dentry *, char *, char *);

/* in file ramread.c */
extern	devcall	ramread(struct dentry *, char *, int32);

/* in file ramwrite.c */
extern	devcall	ramwrite(struct dentry *, char *, int32);

/* in file read.c */
extern	syscall	read(did32, char *, uint32);

/* in file ready.c */
extern	status	ready(pid32);

/* in file receive.c */
extern	umsg32	receive(void);

/* in file recvclr.c */
extern	umsg32	recvclr(void);

/* in file recvtime.c */
extern	umsg32	recvtime(int32);

/* in file resched.c */
extern	void	resched(void);
extern	status	resched_cntl(int32);

/* in file intutils.S */
extern	void	restore(intmask);

/* in file resume.c */
extern	pri16	resume(pid32);

/* in file seek.c */
extern	syscall	seek(did32, uint32);

/* in file semcount.c */
extern	syscall	semcount(sid32);

/* in file semcreate.c */
extern	sid32	semcreate(int32);

/* in file semdelete.c */
extern	syscall	semdelete(sid32);

/* in file semreset.c */
extern	syscall	semreset(sid32, int32);

/* in file send.c */
extern	syscall	send(pid32, umsg32);

/* in file shell.c */
extern 	process shell(did32);

/* in file signal.c */
extern	syscall	signal(sid32);

/* in file signaln.c */
extern	syscall	signaln(sid32, int32);

/* in file sleep.c */
extern	syscall	sleepms(int32);
extern	syscall	sleep(int32);

/* in file start.S */
extern	int32	inb(int32);
extern	int32	inw(int32);
extern	int32	inl(int32);
extern	int32	outb(int32, int32);
extern	int32	outw(int32, int32);
extern	int32	outl(int32, int32);
extern	int32	insw(int32, int32 ,int32);
extern	int32	outsw(int32, int32, int32);
extern	int32	lidt(void);
extern	int32	cpuid(void);

/* in file suspend.c */
extern	syscall	suspend(pid32);

/* in file ttycontrol.c */
extern	devcall	ttycontrol(struct dentry *, int32, int32, int32);

/* in file ttydispatch.c */
extern	interrupt	ttydispatch(void);

/* in file ttygetc.c */
extern	devcall	ttygetc(struct dentry *);

/* in file ttyhandle_in.c */
extern	void	ttyhandle_in(struct ttycblk *, struct uart_csreg *);

/* in file ttyhandle_out.c */
extern	void	ttyhandle_out(struct ttycblk *, struct uart_csreg *);

/* in file ttyhandler.c */
extern	void	ttyhandler(void);

/* in file ttyinit.c */
extern	devcall	ttyinit(struct dentry *);

/* in file ttykickout.c */
extern	void	ttykickout(struct uart_csreg *);

/* in file ttyputc.c */
extern	devcall	ttyputc(struct dentry *, char);

/* in file ttyread.c */
extern	devcall	ttyread(struct dentry *, char *, int32);

/* in file ttywrite.c */
extern	devcall	ttywrite(struct dentry *, char *, int32);

/* in file unsleep.c */
extern	syscall	unsleep(pid32);

/* in file userret.c */
extern	void	userret(void);

/* in file wait.c */
extern	syscall	wait(sid32);

/* in file wakeup.c */
extern	void	wakeup(void);

/* in file write.c */
extern	syscall	write(did32, char *, uint32);

/* in file xdone.c */
extern	void	xdone(void);

/* in file yield.c */
extern	syscall	yield(void);

/* in file lmfinit.c */
extern	devcall	lmfinit(struct dentry *);

/* in file lmfctrl.c */
extern	devcall	lmfctrl(struct dentry *, int32, int32, int32);

/* in file lmfopen.c */
extern	devcall	lmfopen(struct dentry *, char *, char *);

/* in file lfinit.c */
extern	devcall	lfinit(struct dentry *);

/* in file lfclose.c */
extern	devcall	lfclose(struct dentry *);

/* in file lfgetc.c */
extern	devcall	lfgetc(struct dentry *);

/* in file lfputc.c */
extern	devcall	lfputc(struct dentry *, char);

/* in file lfread.c */
extern	devcall	lfread(struct dentry *, char *, int32);

/* in file lfseek.c */
extern	devcall	lfseek(struct dentry *, int32);

/* in file lfwrite.c */
extern	devcall	lfwrite(struct dentry *, char *, int32);

/* in file pipinit.c */
extern	devcall	pipinit(struct dentry *);

/* in file pipopen.c */
extern	devcall	pipopen(void);

/* in file pipclose.c */
extern	devcall	pipclose(struct dentry *);

/* in file pipgetc.c */
extern	devcall	pipgetc(struct dentry *);

/* in file pipputc.c */
extern	devcall	pipputc(struct dentry *, char);

/* in file pipread.c */
extern	devcall	pipread(struct dentry *);

/* in file pipwrite.c */
extern	devcall	pipwrite(struct dentry *, char *, int32);

/* NETWORK BYTE ORDER CONVERSION NOT NEEDED ON A BIG-ENDIAN COMPUTER */
#define	htons(x)  ((0xff & ((x)>>8)) | ((0xff & (x)) << 8))
#define	htonl(x)  ((((x)>>24) & 0x000000ff) | (((x)>> 8) & 0x0000ff00) | \
		   (((x)<<8) & 0x00ff0000) | (((x)<<24) & 0xff000000))
#define	ntohs(x)  ((0xff & ((x)>>8)) | ( (0xff & (x)) << 8))
#define	ntohl(x)  ((((x)>>24) & 0x000000ff) | (((x)>> 8) & 0x0000ff00) | \
		   (((x)<<8) & 0x00ff0000) | (((x)<<24) & 0xff000000))
