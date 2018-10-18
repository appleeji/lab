#include <linux/version.h>
#include <generated/autoconf.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/uaccess.h>  /* copy_form_user() */
#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/mutex.h>

#include <asm/uaccess.h>	/* copy_*_user */

#include "scull.h"			/* local definitions */

#define IOCTL_DEV_ID 0x8A /* magic number (decided by developers) */
#define IOCTL_RESET _IO (IOCTL_DEV_ID, 0) /* Reset */
#define IOCTL_GETSIZE _IOR (IOCTL_DEV_ID, 1, int) /* GetSize */
#define IOCTL_SETSIZE _IOW (IOCTL_DEV_ID, 2, int) 

/*
 * Our parameters which can be set at load time.
 */

int scull_major =   SCULL_MAJOR;
int scull_minor =   0;
int scull_nr_devs = SCULL_NR_DEVS;			/* number of bare scull devices */
int scull_quantum = SCULL_QUANTUM;
int scull_qset =    SCULL_QSET;
module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);
module_param(scull_nr_devs, int, S_IRUGO);
module_param(scull_quantum, int, S_IRUGO);
module_param(scull_qset, int, S_IRUGO);

MODULE_AUTHOR("Alessandro Rubini, Jonathan Corbet");
MODULE_LICENSE("Dual BSD/GPL");

struct scull_dev *scull_devices;	/* allocated in scull_init_module */

/*
 * Empty out the scull device; must be called with the device
 * semaphore held.
 */
int scull_trim(struct scull_dev *dev) {
	struct scull_qset *next, *dptr;
	int qset = dev->qset;   /* "dev" is not-null */
	int i;

	for (dptr = dev->data; dptr; dptr = next) { /* all the list items */
		if (dptr->data) {
			for (i = 0; i < qset; i++)
				kfree(dptr->data[i]);
			kfree(dptr->data);
			dptr->data = NULL;
		}
		next = dptr->next;
		kfree(dptr);
	}
	dev->size = 0;
	dev->quantum = scull_quantum;
	dev->qset = scull_qset;
	dev->data = NULL;
	return 0;
}

/*
 * Open and close
 */
int scull_open(struct inode *inode, struct file *filp) {
	/*TODO: Write your codes */ 
	struct scull_dev *dev;
	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev;
	return 0;          /* success */
}

int scull_release(struct inode *inode, struct file *filp) {
    /*TODO: Write your codes */ 

    return 0;
}

/*
 * Follow the list
 */
struct scull_qset *scull_follow(struct scull_dev *dev, int n) {
	struct scull_qset *qs = dev->data;

        /* Allocate first qset explicitly if need be */
	if (! qs) {
		qs = dev->data = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
		if (qs == NULL)
			return NULL;  /* Never mind */
		memset(qs, 0, sizeof(struct scull_qset));
	}

	/* Then follow the list */
	while (n--) {
		if (!qs->next) {
			qs->next = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
			if (qs->next == NULL)
				return NULL;  /* Never mind */
			memset(qs->next, 0, sizeof(struct scull_qset));
		}
		qs = qs->next;
		continue;
	}
	return qs;
}

/*
 * Data management: read and write
 */
ssize_t scull_read(struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos) {
	struct scull_dev *dev = filp->private_data; 
	struct scull_qset *dptr;	/* the first listitem */
	int quantum = dev->quantum, qset = dev->qset;
	int itemsize = quantum * qset; /* how many bytes in the listitem */
	int item, s_pos, q_pos, rest;
	ssize_t retval = 0;

	/*TODO: Write your codes */ 
	mutex_lock (&dev->mut);
	if (*f_pos < dev->ndata) {
		if (*f_pos + count > dev->ndata)
			count = dev->ndata - *f_pos;
		if (copy_to_user(buf, dev->data + *f_pos, count)) {
			retval = -EFAULT;
		} else {
			*f_pos += count;
			retval = count; /* count bytes have been read */
		}
	} else {
		retval = 0; /* zero bytes have been read from the device */
	}


	mutex_unlock (&dev->mut);

    return retval;
}

ssize_t scull_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos) {
    struct scull_dev *dev = filp->private_data;
	struct scull_qset *dptr;
	int quantum = dev->quantum, qset = dev->qset;
	int itemsize = quantum * qset;
	int item, s_pos, q_pos, rest;
	ssize_t retval = -ENOMEM; /* value used in "goto out" statements */

	/*TODO: Write your codes */ 
	mutex_lock (&dev->mut);
	if (*f_pos < dev->size) {
		if (*f_pos + count > dev->size)
			count = dev->size - *f_pos;
		if (copy_from_user(dev->data + *f_pos, buf, count)) {
			retval = -EFAULT;
		} else {
			*f_pos += count;
			dev->ndata = *f_pos;
			retval = count;
		}
	} else {
		retval = 0;
	}


	mutex_unlock (&dev->mut);


    return retval;
}

/*
 * The ioctl() implementation
 */
long scull_ioctl(struct file *filp,
                 unsigned int cmd, unsigned long arg) {
 //   int err = 0, tmp;
  //  long retval = 0;

     /*TODO: Write your codes */ 
    struct scull_dev *dev = filp->private_data;
    int err = 0, retval = 0;
    switch (cmd) {
	    case IOCTL_RESET:
		    dev->ndata = 0;
		    break ;
	    case IOCTL_GETSIZE:
		    retval = __put_user(dev->ndata, (int __user *)arg);
		    break ;
	    case IOCTL_SETSIZE:
		    /* allow changes only to superuser */
		    retval = __get_user(dev->ndata, (int __user *)arg);
		    break ;
	    default:
		    retval = -ENOTTY;
    }

    return retval;
}

/*
 * The "extended" operations -- only seek
 */
loff_t scull_llseek(struct file *filp, loff_t off, int whence) {
	struct scull_dev *dev = filp->private_data;
	loff_t newpos;

	switch(whence) {
	  case 0: /* SEEK_SET */
		newpos = off;
		break;

	  case 1: /* SEEK_CUR */
		newpos = filp->f_pos + off;
		break;

	  case 2: /* SEEK_END */
		newpos = dev->size + off;
		break;

	  default: /* can't happen */
		return -EINVAL;
	}
	if (newpos < 0) return -EINVAL;
	filp->f_pos = newpos;
	return newpos;
}

struct file_operations scull_fops = {
  /*TODO: Write your codes */ 
	.owner= THIS_MODULE,
	.llseek = scull_llseek,
	.read = scull_read,
	.write = scull_write,
	.unlocked_ioctl = scull_ioctl,
//	.ioctl = scull_ioctl,
	.open = scull_open,
	.release = scull_release,
};

/*
 * The cleanup function is used to handle initialization failures as well.
 * Thefore, it must be careful to work correctly even if some of the items
 * have not been initialized
 */
void scull_cleanup_module(void) {
	int i;
	dev_t devno = MKDEV(scull_major, scull_minor);

	/* Get rid of our char dev entries */
	if (scull_devices) {
		for (i = 0; i < scull_nr_devs; i++) {
			scull_trim(scull_devices + i);
			cdev_del(&scull_devices[i].cdev);
		}
		kfree(scull_devices);
	}

	/* cleanup_module is never called if registering failed */
	unregister_chrdev_region(devno, scull_nr_devs);
}

/*
 * Set up the char_dev structure for this device.
 */
static void scull_setup_cdev(struct scull_dev *dev, int index) {
  /*TODO: Write your codes */ 
	int err, devno;
	int dev_size = 1024;
	dev->data = kmalloc (dev_size, GFP_KERNEL);
	dev->size = dev_size;
	dev->ndata = 0;
	mutex_init(&dev->mut);

	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;

	devno = MKDEV(scull_major, scull_minor);
	err = cdev_add(&dev->cdev, devno, 1);
	/* fail gracefully if need be */

	if (err) {
		printk(KERN_NOTICE "Error %d adding scull%d", err, devno);
	}
}

int scull_init_module(void) {
	int result = 0, i;
//	dev_t dev = 0;
	struct scull_dev dev;

    /*TODO: Write your codes */ 
	scull_setup_cdev(&dev,0);

    return 0; /* succeed */

  fail:
	scull_cleanup_module();
	return result;
}

module_init(scull_init_module);
module_exit(scull_cleanup_module);
