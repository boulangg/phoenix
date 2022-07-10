#pragma once

#include <list>
#include <vector>

#include <mm/Page.hpp>

class Inode;

class AddressSpace {
public:
	AddressSpace(Inode* host);

	Page* getPage(size_t pageNo);
	virtual ~AddressSpace();

	int readPage(Page *p) {
		return readPage_internal(p);
	}


	// adress_space_operation
	//int (*writepage)(struct page *page, struct writeback_control *wbc);
private:
	virtual int readPage_internal(Page* p) = 0;
	//int set_page_dirty(Page *page);

	/*int (*writepages)(struct address_space *, struct writeback_control *);
	int (*set_page_dirty)(struct page *page);
	int (*readpages)(struct file *filp, struct address_space *mapping,
			struct list_head *pages, unsigned nr_pages);
	int (*write_begin)(struct file *, struct address_space *mapping,
				loff_t pos, unsigned len, unsigned flags,
				struct page **pagep, void **fsdata);
	int (*write_end)(struct file *, struct address_space *mapping,
				loff_t pos, unsigned len, unsigned copied,
				struct page *page, void *fsdata);
	sector_t (*bmap)(struct address_space *, sector_t);
	void (*invalidatepage) (struct page *, unsigned int, unsigned int);
	int (*releasepage) (struct page *, int);
	void (*freepage)(struct page *);
	ssize_t (*direct_IO)(struct kiocb *, struct iov_iter *iter);*/
	/* isolate a page for migration */
	//bool (*isolate_page) (struct page *, isolate_mode_t);
	/* migrate the contents of a page to the specified target */
	//int (*migratepage) (struct page *, struct page *);
	/* put migration-failed page back to right list */
	//void (*putback_page) (struct page *);
	//int (*launder_page) (struct page *);

	//int (*is_partially_uptodate) (struct page *, unsigned long,
	//				unsigned long);
	//void (*is_dirty_writeback) (struct page *, bool *, bool *);
	//int (*error_remove_page) (struct mapping *mapping, struct page *page);
	//int (*swap_activate)(struct file *);
	//int (*swap_deactivate)(struct file *);

	// Help
	//Page* p;

protected:
	Inode* host;
};
