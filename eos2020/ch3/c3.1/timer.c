struct sp804 {
	volatile int load;
	volatile int value;
	volatile int cntl;
	volatile int intclr;
	volatile int ris;
	volatile int mis;
	volatile int bgload;
};

typedef struct timer {
	struct sp804 *base;
	int tick, hh, mm, ss;
	char clock[16];
} TIMER;

TIMER timer[4] = {
	[0] = {
		.base = (void*)0x101e2000,
	},
	[1] = {
		.base = (void*)0x101e2020,
	},
	[2] = {
		.base = (void*)0x101e3000,
	},
	[3] = {
		.base = (void*)0x101e3020,
	}
};

void timer_clearInterrupt(int n);

void timer_init()
{
	int i;
	TIMER *tp;
	struct sp804 *base;
	kprintf("%s()\n", __func__);
	for (i = 0; i < ARRAY_SIZE(timer); i++) {
		tp = timer + i;
		base = tp->base;
		base->load = 0;
		base->value = 0xffffffff;//.value is 32bit RO, so what's the meaning to write them?
		base->ris = 0;
		base->mis = 0;//both ris and mis are of read type, so what's the meaning to write them?
		base->load = 0x100;
		base->cntl = 0x62;//0110 0010:DIS|periodic|IntEnabled|-,/1|32bit counter|wrapping(rather than one shot)
		base->bgload = 0xf0000;//the value(to be decremented) used to reload the counter when periodic mode is enabled
		tp->tick = tp->hh = tp->mm = tp->ss = 0;
		strcpy((char*)tp->clock, "00:00:00");//-> is higher than (type)
	}
}

void timer_handler(int n)
{
	int i;
	TIMER *t = timer + n;
	t->tick++;
	t->ss = t->tick;
	t->ss %= 60;
	if (t->ss == 0) {
		t->mm++;
		if (t->mm % 60 == 0) {//% is higher than ==
			t->mm = 0;
			t->hh++;
		}
	}
	for (i = 0; i < 8; i++) {
		unkpchar(t->clock[i], n, 70 + i);
	}
	t->clock[7] = '0' + (t->ss % 10);
	t->clock[6] = '0' + (t->ss / 10);
	t->clock[4] = '0' + (t->mm % 10);
	t->clock[3] = '0' + (t->mm / 10);
	t->clock[1] = '0' + (t->hh % 10);
	t->clock[0] = '0' + (t->hh / 10);
	color = GREEN;
	for (i = 0; i < 8; i++)
		kpchar(t->clock[i], n , 70 + i);
	timer_clearInterrupt(n);
}

void timer_start(int n)
{
	TIMER *tp = &timer[n];
	kprintf("%s %d base=%x\n", __func__, n, tp->base);
	tp->base->cntl |= 0x80;//1<<7: timer enabled 0: timer disabled
}

void timer_clearInterrupt(int n)
{
	TIMER *tp = timer + n;
	tp->base->intclr = 0xffffffff;//any write to the register clears the interrupt output from the counter
}
