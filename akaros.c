#include <parlib.h>
#include <unistd.h>
#include <alarm.h>
#include "kweb.h"
/* including for the header values for faking support to the rest of kweb */
#include "tpool.h"
#include "kstats.h"

int usleep(useconds_t usec)
{
	sys_block(usec);
	/* TODO: should check for syscall abortion and error out.  man usleep */
	return 0;
}

void init_connection(struct http_connection *c)
{
}

void destroy_connection(struct http_connection *c)
{
}

ssize_t timed_read(struct http_connection *c, void *buf, size_t count)
{
	ssize_t ret;
	struct alarm_waiter waiter;

	init_awaiter(&waiter, alarm_abort_sysc);
	waiter.data = current_uthread;

	set_awaiter_rel(&waiter, KWEB_SREAD_TIMEOUT * 1000);
	set_alarm(&waiter);

	tpool_inform_blocking(&tpool);
	ret = read(c->socketfd, buf, count);
	tpool_inform_unblocked(&tpool);

	unset_alarm(&waiter);
	return ret;
}

ssize_t timed_write(struct http_connection *c, const void *buf, size_t count)
{
	ssize_t ret;
	struct alarm_waiter waiter;

	init_awaiter(&waiter, alarm_abort_sysc);
	waiter.data = current_uthread;

	set_awaiter_rel(&waiter, KWEB_SREAD_TIMEOUT * 1000);
	set_alarm(&waiter);

	tpool_inform_blocking(&tpool);
	ret = write(c->socketfd, buf, count);
	tpool_inform_unblocked(&tpool);

	unset_alarm(&waiter);
	return ret;
}


/* Faking tpool and kstats */

void tpool_resize(struct tpool *t, int size)
{
	printf("Got resize request, skipping\n");
}

int tpool_init(struct tpool *t, int size, struct kqueue *q,
               void (*func)(struct kqueue *, struct kitem *), size_t stacksize)
{
	return 0;
}

void tpool_inform_blocking(struct tpool *t)
{
}

void tpool_inform_unblocked(struct tpool *t)
{
}

void tpool_wake(struct tpool *t, int count)
{
}

void kstats_init(struct kstats *kstats, struct kqueue *kqueue,
                 struct tpool *tpool, struct cpu_util *cpu_util)
{
}

int kstats_start(struct kstats *kstats, unsigned int period_ms)
{
	return -1;
}

int kstats_stop(struct kstats *kstats)
{
	return -1;
}

void kstats_print_lifetime_statistics(struct kstats *kstats)
{
}
