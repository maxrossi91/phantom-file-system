// Copyright (c) 2020, Stefano Aldegheri, Enrico Fraccaroli, and, Massimiliano Rossi.  All rights reserved.
// Use of this source code is governed by a GPLv3.0 license that can be found in the LICENSE file.

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/pagemap.h>



#define PHFS_MAGIC     0x6f584885

MODULE_LICENSE("GPL3.0");
MODULE_AUTHOR("Stefano Aldegheri, Enrico Fraccaroli, and, Massimiliano Rossi.");
MODULE_DESCRIPTION("With this file system, you are able to navigate what is not physically reachable anymore (already detached mountable data storages).");
MODULE_VERSION("0.01");


static struct inode *phfs_get_inode(struct super_block *sb, int mode)
{
	struct inode *ret = new_inode(sb);

	if (ret) {
    kgid_t gid;
    kuid_t uid;
		ret->i_mode = mode;
		ret->i_uid = uid;
    ret->i_gid = gid;
		//ret->i_blksize = PAGE_CACHE_SIZE;
		ret->i_blocks = 0;
		//ret->i_atime = ret->i_mtime = ret->i_ctime = CURRENT_TIME;
	}
	return ret;
}


// /*
//  * Display the mount options in /proc/mounts.
//  */
// static int phfs_show_options(struct seq_file *m, struct dentry *root)
// {
// 	struct phfs_fs_info *fsi = root->d_sb->s_fs_info;
// 	if (fsi->mount_opts.mode != PHFS_DEFAULT_MODE)
// 		seq_printf(m, ",mode=%o", fsi->mount_opts.mode);
// 	return 0;
// }

static const struct super_operations phfs_ops = {
        .statfs         = simple_statfs,
        .drop_inode     = generic_delete_inode,
        // .show_options   = phfs_show_options,
};

static int phfs_fill_super(struct super_block *sb, void *data, int silent)
{
        // struct phfs_fs_info *fsi;
        struct inode *inode;
        int err;

        // save_mount_options(sb, data);
        //
        // fsi = kzalloc(sizeof(struct phfs_fs_info), GFP_KERNEL);
        // sb->s_fs_info = fsi;
        // if (!fsi)
        //         return -ENOMEM;
        //
        // err = phfs_parse_options(data, &fsi->mount_opts);
        // if (err)
        //         return err;

        sb->s_maxbytes          = MAX_LFS_FILESIZE;
        sb->s_blocksize         = PAGE_SIZE;
        sb->s_blocksize_bits    = PAGE_SHIFT;
        sb->s_magic             = PHFS_MAGIC;
        sb->s_op                = &phfs_ops;
        sb->s_time_gran         = 1;

        inode = phfs_get_inode(sb, 0755);
        sb->s_root = d_make_root(inode);
        if (!sb->s_root)
                return -ENOMEM;

        return 0;
}

// struct super_block {
//         //...
//         dev_t                   s_dev;              /* identifier */
//         unsigned char           s_blocksize_bits;   /* block size in bits */
//         unsigned long           s_blocksize;        /* block size in bytes */
//         unsigned char           s_dirt;             /* dirty flag */
//         loff_t                  s_maxbytes;         /* max file size */
//         struct file_system_type *s_type;            /* filesystem type */
//         struct super_operations *s_op;              /* superblock methods */
//         //...
//         unsigned long           s_flags;            /* mount flags */
//         unsigned long           s_magic;            /* filesystem’s magic number */
//         struct dentry           *s_root;            /* directory mount point */
//         //...
//         char                    s_id[32];           /* informational name */
//         void                    *s_fs_info;         /* filesystem private info */
// };

// TODO: phfs_kill_litter_super

// TODO: phfs_kill_sb

static struct dentry *phfs_mount(struct file_system_type *fs_type,
        int flags, const char *dev_name, void *data)
{
        return mount_nodev(fs_type, flags, data, phfs_fill_super);
}

static struct file_system_type phfs_fs_type = {
  .name		= "phfs",
  .owner		= THIS_MODULE,
	.mount		= phfs_mount,
  .kill_sb	= kill_block_super,//phfs_kill_sb,
	.fs_flags	= FS_USERNS_MOUNT,
};

MODULE_ALIAS_FS("phfs");


// File system registration
static int __init phfs_init(void) {
  printk(KERN_INFO "Phantom File System Init!\n");
  return  register_filesystem(&phfs_fs_type);
}

static void __exit phfs_exit(void) {
  printk(KERN_INFO "Phantom File System Exit!\n");
  unregister_filesystem(&phfs_fs_type);
}

module_init(phfs_init);
module_exit(phfs_exit);
