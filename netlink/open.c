#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>
/*
this program is to test if a fs can be umounted if one of its file has been kept opened:
ordinary umount(,0), ie cmd line's "umount /mntpoint" cannot work but umount(,2) can!
build:
~/openwrt_CC_mt76xx_zhuotk_source/staging_dir/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-gcc -Wall open.c -o open.out
scp open.out root@192.168.12.251:/ 

test1:
root@ZhuoTK:/tmp# mount
rootfs on / type rootfs (rw)
/dev/root on /rom type squashfs (ro,relatime)
proc on /proc type proc (rw,nosuid,nodev,noexec,noatime)
sysfs on /sys type sysfs (rw,nosuid,nodev,noexec,noatime)
tmpfs on /tmp type tmpfs (rw,nosuid,nodev,noatime)
/dev/mtdblock6 on /overlay type jffs2 (rw,noatime)
overlayfs:/overlay on / type overlay (rw,noatime,lowerdir=/,upperdir=/overlay/upper,workdir=/overlay/work)
tmpfs on /dev type tmpfs (rw,nosuid,relatime,size=512k,mode=755)
debugfs on /sys/kernel/debug type debugfs (rw,noatime)
root@ZhuoTK:/tmp# mkdir -p /tmp/tmp
root@ZhuoTK:/tmp# mount -t tmpfs tmpfs /tmp/tmp
[ 1889.990000] SYSC_mount("tmpfs", "/tmp/tmp", "tmpfs", 8000,   (null)): 0 <--echo 8 >/proc/sys/kernel/printk to show pr_debug for SYSC_mount,etc
root@ZhuoTK:/tmp# mknod /tmp/tmp/kmsg c 1 11
[ 1911.540000] SYSC_mknodat: /tmp/tmp/kmsg, 20666 10b
root@ZhuoTK:/tmp# /open.out /tmp/tmp/kmsg /tmp/tmp/ 0
[ 1928.290000] ***SYSC_umount("/tmp/tmp/", 0): -16<---EBUSY:device busy if don't use MNT_DETACH
open /tmp/tmp/kmsg: 3; umount(/tmp/tmp/, 0): -1
[ 1932.940000] uart_flush_buffer(0) called
^C	<---exits program's sleep
root@ZhuoTK:/tmp# ls tmp/ -l
crw-r--r--    1 root     root        1,  11 Aug 20 04:37 kmsg<---since /tmp/tmp has not been umounted, so its files still exists
root@ZhuoTK:/tmp# /open.out /tmp/tmp/kmsg /tmp/tmp/ &
root@ZhuoTK:/tmp# [ 1951.300000] SYSC_umount("/tmp/tmp/", 2): 0<--umount with MNT_DETACH will success
open /tmp/tmp/kmsg: 3; umount(/tmp/tmp/, 2): 0<--both open/umount syscall success

root@ZhuoTK:/tmp# mount<---/tmp/tmp has been removed and it cannot be seen
rootfs on / type rootfs (rw)
/dev/root on /rom type squashfs (ro,relatime)
proc on /proc type proc (rw,nosuid,nodev,noexec,noatime)
sysfs on /sys type sysfs (rw,nosuid,nodev,noexec,noatime)
tmpfs on /tmp type tmpfs (rw,nosuid,nodev,noatime)
/dev/mtdblock6 on /overlay type jffs2 (rw,noatime)
overlayfs:/overlay on / type overlay (rw,noatime,lowerdir=/,upperdir=/overlay/upper,workdir=/overlay/work)
tmpfs on /dev type tmpfs (rw,nosuid,relatime,size=512k,mode=755)
debugfs on /sys/kernel/debug type debugfs (rw,noatime)
root@ZhuoTK:/tmp# ls /tmp/tmp/<---there will be no /tmp/tmp/kmsg since it has been umounted
root@ZhuoTK:/tmp# 

test2:
root@ZhuoTK:/tmp# mount
rootfs on / type rootfs (rw)
/dev/root on /rom type squashfs (ro,relatime)
proc on /proc type proc (rw,nosuid,nodev,noexec,noatime)
sysfs on /sys type sysfs (rw,nosuid,nodev,noexec,noatime)
tmpfs on /tmp type tmpfs (rw,nosuid,nodev,noatime)
/dev/mtdblock6 on /overlay type jffs2 (rw,noatime)
overlayfs:/overlay on / type overlay (rw,noatime,lowerdir=/,upperdir=/overlay/upper,workdir=/overlay/work)
tmpfs on /dev type tmpfs (rw,nosuid,relatime,size=512k,mode=755)
debugfs on /sys/kernel/debug type debugfs (rw,noatime)
tmpfs on /tmp/tmp type tmpfs (rw,relatime)<---mount -t tmpfs tmpfs /tmp/tmp
root@ZhuoTK:/tmp# mknod tmp/kmsg c 1 11
[ 2288.810000] SYSC_mknodat: tmp/kmsg, 20666 10b
root@ZhuoTK:/tmp# /open.out /tmp/tmp/kmsg /tmp/tmp/ 0 &
root@ZhuoTK:/tmp# [ 2315.930000] ***SYSC_umount("/tmp/tmp/", 0): -16<---umount fails since open.out has opend /tmp/tmp/kmsg of /tmp/tmp
open /tmp/tmp/kmsg: 3; umount(/tmp/tmp/, 0): -1

root@ZhuoTK:/tmp# mount
rootfs on / type rootfs (rw)
/dev/root on /rom type squashfs (ro,relatime)
proc on /proc type proc (rw,nosuid,nodev,noexec,noatime)
sysfs on /sys type sysfs (rw,nosuid,nodev,noexec,noatime)
tmpfs on /tmp type tmpfs (rw,nosuid,nodev,noatime)
/dev/mtdblock6 on /overlay type jffs2 (rw,noatime)
overlayfs:/overlay on / type overlay (rw,noatime,lowerdir=/,upperdir=/overlay/upper,workdir=/overlay/work)
tmpfs on /dev type tmpfs (rw,nosuid,relatime,size=512k,mode=755)
debugfs on /sys/kernel/debug type debugfs (rw,noatime)
tmpfs on /tmp/tmp type tmpfs (rw,relatime)<---/tmp/tmp is not successfully umounted
root@ZhuoTK:/tmp# umount /tmp/tmp/
[ 2324.130000] ***SYSC_umount("/tmp/tmp", 0): -16
umount: can't umount /tmp/tmp: Device or resource busy<---it is equals to umount(,0) so it fails like the above
root@ZhuoTK:/tmp# mount
rootfs on / type rootfs (rw)
/dev/root on /rom type squashfs (ro,relatime)
proc on /proc type proc (rw,nosuid,nodev,noexec,noatime)
sysfs on /sys type sysfs (rw,nosuid,nodev,noexec,noatime)
tmpfs on /tmp type tmpfs (rw,nosuid,nodev,noatime)
/dev/mtdblock6 on /overlay type jffs2 (rw,noatime)
overlayfs:/overlay on / type overlay (rw,noatime,lowerdir=/,upperdir=/overlay/upper,workdir=/overlay/work)
tmpfs on /dev type tmpfs (rw,nosuid,relatime,size=512k,mode=755)
debugfs on /sys/kernel/debug type debugfs (rw,noatime)
tmpfs on /tmp/tmp type tmpfs (rw,relatime)<---/tmp/tmp is not successfully umounted
root@ZhuoTK:/tmp# umount -l /tmp/tmp/<--- -l lazy cannot works as expected
[ 2374.710000] ***SYSC_umount("/tmp/tmp", 0): -16
[ 2374.720000] SYSC_umount("/tmp/tmp", 2): 0
*/
int main(int argc, char **argv)
{
	int fd = open(argv[1], O_RDWR | O_APPEND);
/*
MNT_DETACH (since Linux 2.4.11)
  Perform a lazy unmount: make the mount point unavailable for new accesses, immediately disconnect the filesystem and all
  filesystems mounted below it from each other and from the mount table, and actually perform the unmount when the mount point
  ceases(to stop doing sth) to be busy.
*/
	int ret, flags = MNT_DETACH;
	if (argc > 3 && argv[3]) {
		flags = atoi(argv[3]);
	}
	ret = umount2(argv[2], flags);
	printf("open %s: %d; umount(%s, %d): %d\n", argv[1], fd, argv[2], flags, ret);
	sleep(65536);
	printf("sleep fins\n");
	close(fd);
	return 0;
}