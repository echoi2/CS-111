//NAME:Eugene Choi
//EMAIL:echoi2@g.ucla.edu
//ID:905368197

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>
#include "ext2_fs.h"

#define BOOT_BLOCK_SIZE EXT2_MIN_BLOCK_SIZE //The offset before the superblock 
int imgfd;
__u32 block_size; //used to convert the block_size value that's inside the superblock class into bytes
struct ext2_super_block super;
struct ext2_group_desc group;
struct ext2_inode inode;
struct ext2_dir_entry direct;

void read_and_print_super_block();
void read_and_print_group();
void read_and_print_free_block_bitmap();
void read_and_print_free_inode_bitmap();
void read_and_print_inode_summary();
void read_and_print_directory_inode(__u32 parent_inode_num, int k);
void read_and_print_indirect_blocks(__u32 parent_inode_num, int block_loc, int offset,int level);

int main(int argc, char * argv[]){
	if(argc != 2){
		fprintf(stderr, "Incorrect number of arguments. Please try again.\n");
		exit(1);
	}
	if((imgfd = open(argv[1], O_RDONLY)) == -1){
		fprintf(stderr, "Error: File provided could not be opened.\n");
		exit(1);
	}

	read_and_print_super_block();
	read_and_print_group();
	read_and_print_free_block_bitmap();
	read_and_print_free_inode_bitmap();
	read_and_print_inode_summary();
	//printf("%d\n", sizeof(group));

	/*printf("%d\n", BOOT_BLOCK_SIZE);
	printf("%d\n", EXT2_MIN_BLOCK_SIZE);
	*/
	exit(0);
}

void read_and_print_super_block(){ //used to read in values and populate the superblock struct (super) and to print out the now populated values from pread onto stdout
	if(pread(imgfd, &super, sizeof(super), BOOT_BLOCK_SIZE) == -1){
		fprintf(stderr, "Error: Could not read bytes from superblock.\n");
		exit(2);
	}
	block_size = 1024 << super.s_log_block_size;
	fprintf(stdout, "SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n", super.s_blocks_count, super.s_inodes_count, block_size, super.s_inode_size, super.s_blocks_per_group, super.s_inodes_per_group, super.s_first_ino);
	/*printf("%d\n", block_size);
	printf("%d\n", super.s_log_block_size);
	printf("%d\n", super.s_inodes_count);
	*/
}

void read_and_print_group(){
	//We don't have to consider multiple offsets the groups because it is given to us in the specs that all the tests will only have 1 group.
	if(pread(imgfd, &group, sizeof(group), BOOT_BLOCK_SIZE + sizeof(super)) == -1){
		fprintf(stderr, "Error: Could not read bytes into group descriptor.\n");
		exit(2);
	}
	//In our case, super.s_blocks_count is equal to the total number of blocks in the current group because we only have 1 group. (Also know from spec that all the test imgs will be 1 group) 
	//The super block also tells us the number of blocks per group with s_blocks_per_group. In our case, s_blocks_per_group for trivial.img is 8192. Therefore,
	//since the value for s_blocks_size for trivial.img is 64, there can only be one group.
	//referenced from http://cs.smith.edu/~nhowe/Teaching/csc262/oldlabs/ext2.html 
	fprintf(stdout, "GROUP,0,%d,%d,%d,%d,%d,%d,%d\n", super.s_blocks_count, super.s_inodes_count, group.bg_free_blocks_count, group.bg_free_inodes_count, group.bg_block_bitmap, group.bg_inode_bitmap, group.bg_inode_table);
}

void read_and_print_free_block_bitmap(){
	__u32 i = 0;
	int j = 0;
	char * bitmap = malloc(block_size); //In the case that block size is 1024, bitmap will begin at block 3
	if (pread(imgfd, bitmap, block_size, block_size * group.bg_block_bitmap) == -1){ //read in bytes from block 3 onward to block 4 (in the case of 1 group)
		fprintf(stderr, "Error: Could not read bytes to bitmap.\n");
		exit(2);
	}
	//printf("%d\n", bitmap[4]);
	for(; i < block_size; i++){ //for parsing by byte
		//printf("Doing byte %d\n", i);
		for(j = 0; j < 8; j++){ //for parsing by individual bit
			//printf("%d\n", ((bitmap[i] & (1 << j)) >> j));
			//i = index, j = offset as seen in the TA notes
			if((((bitmap[i] & (1 << j)) >> j) & 1) == 0){
				fprintf(stdout, "BFREE,%d\n", j + (i * 8) + 1); //each byte has 8 bits so we do block_size * 8 to say that when we have logged every bit in a byte, we go up by a count of 8 to say that 8 block have been covered
																// + 1 is because we start at block 1 because block 0 is always used due to the boot block
			}
		}
	}
	free(bitmap);
}

void read_and_print_free_inode_bitmap(){
	__u32 i = 0;
	int j = 0;
	char * inode_bitmap = malloc(block_size); //In the case that block size is 1024, bitmap will begin at block 3
	if (pread(imgfd, inode_bitmap, block_size, block_size * group.bg_inode_bitmap) == -1){ //read in bytes from block 3 onward to block 4 (in the case of 1 group)
		fprintf(stderr, "Error: Could not read bytes to inode_bitmap.\n");
		exit(2);
	}
	//printf("%d\n", inode_bitmap[0]);
	//printf("%d\n", inode_bitmap[2]);
	for(; i < block_size; i++){ //for parsing by byte
		//printf("Doing byte %d\n", i);
		for(j = 0; j < 8; j++){ //for parsing by individual bit
			//printf("%d\n", ((inode_bitmap[i] & (1 << j)) >> j));
			//i = index, j = offset as seen in the TA notes
			if((((inode_bitmap[i] & (1 << j)) >> j) & 1) == 0){
				fprintf(stdout, "IFREE,%d\n", j + (i * 8) + 1); //each byte has 8 bits so we do block_size * 8 to say that when we have logged every bit in a byte, we go up by a count of 8 to say that 8 blocks have been covered.
																// The + 1 is because we start at block 1 instead of block 0 because block 0 is always used for the boot block
				//printf("Doing byte %d\n", j+(i*8));
			}
		}
	}
	//printf("%d\n", block_size*8);
	free(inode_bitmap);
}

void read_and_print_inode_summary(){
	__u32 i = 0;
	int j = 0;
	char * file_mode = NULL;
	//printf("%d\n", group.bg_inode_table);
	//printf("%d\n", super.s_inode_size);
	for(; i < super.s_inodes_count; i++){
		if(pread(imgfd, &inode, super.s_inode_size, group.bg_inode_table * block_size + (i * sizeof(inode)))== -1){ //starts at block 5
			fprintf(stderr, "Error: Could not read bytes to inode struct.\n");
			exit(2);
		}
		if(inode.i_mode != 0 && inode.i_links_count != 0){
			//https://www.gnu.org/software/libc/manual/html_node/Testing-File-Type.html used to find the return values
			if(S_ISDIR(inode.i_mode) != 0){
				//printf("Got this.\n");
				file_mode = "d";
			}
			else if(S_ISREG(inode.i_mode) != 0){
				//printf("Got this.\n");
				file_mode = "f";
			}
			else if(S_ISLNK(inode.i_mode) != 0){
				//printf("Got this.\n");
				file_mode = "s";
			}
			else{
				//printf("Got this.\n");
				file_mode = "\?";
				//printf("%c\n", *file_mode);
			}



			//https://www.tutorialspoint.com/c_standard_library/c_function_gmtime.htm
			struct tm* atime;
			struct tm* ctime;
			struct tm* mtime;
			time_t time_elapsed_for_atime = inode.i_atime;
			time_t time_elapsed_for_ctime = inode.i_ctime;
			time_t time_elapsed_for_mtime = inode.i_mtime;
			ctime = gmtime(&time_elapsed_for_ctime);
			fprintf(stdout,"INODE,%d,%c,%o,%u,%u,%u,%02d/%02d/%02d %02d:%02d:%02d", i + 1, *file_mode, inode.i_mode & 0xFFF, inode.i_uid, inode.i_gid, inode.i_links_count,
					ctime->tm_mon + 1, ctime->tm_mday, ctime->tm_year%100, ctime->tm_hour, ctime->tm_min, ctime->tm_sec);
			mtime = gmtime(&time_elapsed_for_mtime);
			fprintf(stdout, ",%02d/%02d/%02d %02d:%02d:%02d", mtime->tm_mon + 1, mtime->tm_mday, mtime->tm_year%100, mtime->tm_hour, mtime->tm_min, mtime->tm_sec); 
			atime = gmtime(&time_elapsed_for_atime);
			fprintf(stdout, ",%02d/%02d/%02d %02d:%02d:%02d,%d,%d", atime->tm_mon + 1, atime->tm_mday, atime->tm_year%100, atime->tm_hour, atime->tm_min, atime->tm_sec,
					inode.i_size,inode.i_blocks);



			if(*file_mode == 'd' || *file_mode == 'f'){
				for(j = 0; j < EXT2_N_BLOCKS; j++){
					fprintf(stdout, ",%u",inode.i_block[j]);
				}
				fprintf(stdout, "\n");
				j = 0;

				//directory
				__u32 parent_inode_num = i + 1;
				if(*file_mode == 'd'){
					while(j < EXT2_NDIR_BLOCKS){
				 		if(inode.i_block[j] != 0){
							//printf("%d\n", i+1);
							read_and_print_directory_inode(parent_inode_num, j);
						}
						j++;
					}
				}
				//indirect blocks (used for each file or directory i-node)
				read_and_print_indirect_blocks(parent_inode_num, inode.i_block[EXT2_IND_BLOCK],12,1); 
				read_and_print_indirect_blocks(parent_inode_num, inode.i_block[EXT2_DIND_BLOCK],12 + (block_size/4),2);
				read_and_print_indirect_blocks(parent_inode_num, inode.i_block[EXT2_TIND_BLOCK],12 + (block_size/4) + ((block_size/4)*(block_size/4)),3);

			}



			else if(inode.i_size <= 60 && *file_mode == 's'){ 
				fprintf(stdout, "\n");
			}
		}
	}
}

//struct ext2_dir_entry* direct;
void read_and_print_directory_inode(__u32 parent_inode_num, int k){
	__u32 iter = 0;
	//printf("doing for k value %d\n",k);
	while(iter < block_size){
		if(pread(imgfd, &direct, sizeof(direct), block_size * inode.i_block[k] + iter) == -1){
			fprintf(stderr, "Error: Could not read bytes to directory struct.\n");
			exit(2);
		}
		if(direct.inode != 0){
			//printf("passing here on iter %d\n", iter);
			char file_name[EXT2_NAME_LEN + 1];
			memcpy(file_name, direct.name, direct.name_len);
			//printf("segging here\n");
			//printf("direct %d\n", direct.rec_len);
			file_name[direct.name_len] = 0;
			fprintf(stdout,"DIRENT,%u,%u,%u,%u,%u,'%s'\n",parent_inode_num, iter, direct.inode, direct.rec_len, direct.name_len, file_name);
		}
		iter += direct.rec_len;
	} 
}


void read_and_print_indirect_blocks(__u32 parent_inode_num, int block_loc, int offset, int level){ //when entering this function for the 1st time, level will be 1

	if(block_loc == 0 || level == 0){
		return;
	}
	__u32 ptr_block_value;
	__u32 iter = 0;
	while(iter < block_size){ //same while loop idea shown by ta for parsing directories but just one extra step to have it go to multiple levels
		if(pread(imgfd, &ptr_block_value, sizeof(__u32), block_size * block_loc + iter) == -1){
			fprintf(stderr, "Error: Could not read bytes to ptr_block_value.\n");
			exit(2);
		}
		if(ptr_block_value != 0){
			offset += iter/4;
			fprintf(stdout, "INDIRECT,%d,%d,%d,%d,%d\n", parent_inode_num, level, offset, block_loc, ptr_block_value);
			read_and_print_indirect_blocks(parent_inode_num, ptr_block_value, offset,level - 1);
		}
		iter += 4;
	}
}
