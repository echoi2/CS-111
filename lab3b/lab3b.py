#! /usr/bin/env python3
#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID: 905368197
import sys
import csv
from collections import defaultdict
inode_list = []
dirent_list = []
indirect_list = []
bfree_list = []
ifree_list = []
group = None
superblock = None
inconsistencies = False

#https://micropyramid.com/blog/understand-self-and-__init__-method-in-python-class/ Figuring out how to initialize classes
class ext2_inode: #INODE, inode number, file type, mode, owner, group, link count, time of last I-node change, modification time, time of last access, file size, number of blocks of disk space taken up by this file
                  #in my case here I just pass a ptr to an array of values. Using this ptr I can set values for the constructor by iterating through the array
   def __init__(self, inputs):
      #print(inputs[1])
      self.inode_num = int(inputs[1])
      self.file_type = inputs[2]
      self.mode = int(inputs[3])
      self.owner = int(inputs[4])
      self.group = int(inputs[5])
      self.link_ct = int(inputs[6])
      self.last_change_time = inputs[7]
      self.mod_time = inputs[8]
      self.last_access_time = inputs[9]
      self.file_size = int(inputs[10])
      self.num_of_blocks = inputs[11]
      self.direct_blocks = []
      self.indirect_blocks = []
      #https://www.w3schools.com/python/numpy_array_slicing.asp
      for dir_iterator in inputs[12:24]: #starts from index 12 and goes til 23
         (self.direct_blocks).append(int(dir_iterator))
      for indir_iterator in inputs[24:27]: #starts from index 24 and goes til 27
         (self.indirect_blocks).append(int(indir_iterator))
   
class ext2_dir_entry: #DIRENT, parent inode number, logical byte offset of this entry within the directory, inode number of the referenced file, entry length, name length, name
   def __init__(self, inputs):
      #print(inputs[1])
      self.parent_num = int(inputs[1])
      self.offset = int(inputs[2])
      self.ref_file_inode_num = int(inputs[3])
      self.entry_length = int(inputs[4])
      self.name_length = int(inputs[5])
      self.name = inputs[6]

class ext2_indirect: #INDIRECT, I-node number of the owning file, level of indirection for the block being scanned ... 1 for single indirect, 2 for double indirect, 3 for triple, logical block offset (decimal) represented by the referenced block, block number of the (1, 2, 3) indirect block being scanned, block number of the referenced block
   def __init__(self, inputs):
      #print(inputs[1])
      self.inode_num_owning_file = int(inputs[1])
      self.level = int(inputs[2])
      self.block_offset = int(inputs[3]) #offset repressented by referenced block
      self.block_num_of_scanned_indir_block = int(inputs[4])
      self.block_num_of_referenced_block = int(inputs[5])
      
class ext2_bfree:
   def __init__(self, inputs):
      #print(inputs[1])
      self.free_block_num = int(inputs[1]) #number of free block in the block bitmap

class ext2_ifree:
    def __init__(self, inputs):
      #print(inputs[1]) 
      self.free_inode_num = int(inputs[1]) #number of free inode in the free inode bitmap

class ext2_group: #GROUP, group number, total number of blocks in this group, total number of i-nodes in this group, number of free blocks, number of free i-nodes, block number of free block bitmap for this group, block number of free i-node bitmap for this group, block number of first block of i-nodes in this group
    def __init__(self, inputs):
      #print(inputs[1])
      self.group_num = int(inputs[1])
      self.total_num_of_blocks_in_group = int(inputs[2])
      self.total_num_of_inodes_in_group = int(inputs[3])
      self.num_of_free_blocks = int(inputs[4])
      self.num_of_free_inodes = int(inputs[5])
      self.block_num_of_free_block_bitmap = int(inputs[6])
      self.block_num_of_free_inode_bitmap = int(inputs[7])
      self.first_block_inode = int(inputs[8])
 
class ext2_super:
    def __init__(self, inputs):
      self.total_num_of_blocks = int(inputs[1])
      self.total_num_of_inodes = int(inputs[2])
      self.block_size = int(inputs[3])
      self.inode_size = int(inputs[4])
      self.blocks_per_group = int(inputs[5])
      self.inodes_per_group = int(inputs[6])
      self.first_nonreserved_inode = int(inputs[7])


 
def read_csv( csv_file ):
    with open(csv_file, newline='') as csvfile:
        csv_iterator = csv.reader(csvfile) #makes a reader object that iterates over lines in the given csvfile
        for row in csv_iterator: #used to print every line in the csvfile
           if row[0] == "INODE":
              inode_list.append(ext2_inode(row))
           elif row[0] == "DIRENT":
              dirent_list.append(ext2_dir_entry(row))
           elif row[0] == "INDIRECT":
              indirect_list.append(ext2_indirect(row))
           elif row[0] == "BFREE":
               bfree_list.append(ext2_bfree(row))
           elif row[0] == "IFREE":
               ifree_list.append(ext2_ifree(row))
           elif row[0] == "GROUP":
               add_group(row)
           elif row[0] == "SUPERBLOCK":
               add_super(row)

def add_group( row ):
   global group
   group = ext2_group(row)

def add_super( row ):
   #https://stackoverflow.com/questions/35478504/set-global-variable-in-python-always-return-none-value-in-main
   global superblock #need to specify that superblock is global because otherwise it just creates a local superblock variable and sets the ext2 obj equal to it instead of the global superblock. This caused issues when I tried printing variables from the ext2_super class outside the function
   superblock = ext2_super(row)
   #print(superblock)
   #print(superblock.total_num_of_blocks)

def invalid_print( level, block, inode, offset ):
   global inconsistencies
   inconsistencies = True
   if level == 0:
      #https://stackoverflow.com/questions/15286401/print-multiple-arguments-in-python how to format/print multiple arguments in python
      print("INVALID BLOCK {0} IN INODE {1} AT OFFSET {2}".format(block, inode, offset))
   elif level == 1:
      print("INVALID INDIRECT BLOCK {0} IN INODE {1} AT OFFSET {2}".format(block, inode, offset))
   elif level == 2:
      print("INVALID DOUBLE INDIRECT BLOCK {0} IN INODE {1} AT OFFSET {2}".format(block, inode, offset))
   elif level == 3:
      print("INVALID TRIPLE INDIRECT BLOCK {0} IN INODE {1} AT OFFSET {2}".format(block, inode, offset))

def reserved_print( level, block, inode, offset ):
   global inconsistencies
   inconsistencies = True
   if level == 0:
      print("RESERVED BLOCK {0} IN INODE {1} AT OFFSET {2}".format(block, inode, offset))
   elif level == 1:
      print("RESERVED INDIRECT BLOCK {0} IN INODE {1} AT OFFSET {2}".format(block, inode, offset))
   elif level == 2:
      print("RESERVED DOUBLE INDIRECT BLOCK {0} IN INODE {1} AT OFFSET {2}".format(block, inode, offset))
   elif level == 3:
      print("RESERVED TRIPLE INDIRECT BLOCK {0} IN INODE {1} AT OFFSET {2}".format(block, inode, offset))

def allocated_print( block ):
   global inconsistencies
   inconsistencies = True
   print("ALLOCATED BLOCK {0} ON FREELIST".format(block))

def dup_print(blk_param, inode_param, lvl_param, offset_param):
   global inconsistencies
   inconsistencies = True
   if lvl_param == 0:
      print("DUPLICATE BLOCK {0} IN INODE {1} AT OFFSET {2}".format(blk_param, inode_param, offset_param))
   if lvl_param == 1:
      print("DUPLICATE INDIRECT BLOCK {0} IN INODE {1} AT OFFSET {2}".format(blk_param, inode_param, offset_param))
   if lvl_param == 2:
      print("DUPLICATE DOUBLE INDIRECT BLOCK {0} IN INODE {1} AT OFFSET {2}".format(blk_param, inode_param, offset_param))
   if lvl_param == 3:
      print("DUPLICATE TRIPLE INDIRECT BLOCK {0} IN INODE {1} AT OFFSET {2}".format(blk_param, inode_param, offset_param))

def print_block_consistency():
   offset_1 = 12 #logical offsets for level 1 to 3 indirect blocks
   offset_2 = 268
   offset_3 = 65804
   end_of_reserved = int(group.first_block_inode + ((group.total_num_of_inodes_in_group * superblock.inode_size)/superblock.block_size)) #had to type cast to int b/c when checking all blocks from the end of the reserved blocks to the beginning of the next group block, it was giving me TypeError: 'float' object cannot be interpreted as an integer when input into the range function.
   used_block = {} #creates a dictionary for used blocks
   count = 0
   for node in inode_list:
      offset = 0 #logical offset for direct blocks
      if node.file_type != 's' or node.file_size >= 60:
         for block in node.direct_blocks:
            if block < 0 or block > (superblock.total_num_of_blocks - 1): # need to -1 from superblock.total_num_of_blocks in order to not reference past the maximum number of blocks in the group into the next group block
               invalid_print(0, block, node.inode_num, offset)
            if block < end_of_reserved and block > 0:
               reserved_print(0, block, node.inode_num, offset)
            if block != 0 and block > 0:
               #https://stackoverflow.com/questions/3199171/append-multiple-values-for-one-key-in-a-dictionary
               #essentially marking block as used
               if block not in used_block:
                  used_block[block] = [[block, node.inode_num, 0, offset]] #adding to the used_block list in form of block: [block, inode number, level, offset]
               else:
                  used_block[block].append([block, node.inode_num, 0, offset]) #finds key and appends to the used block list to be in the form of block: [block, inode number, level, offset], [block, inode number, level, offset]
            offset += 1
         
         
         for ind_block in node.indirect_blocks:
            count %=3 #0%3 = 0, 1%3 = 1, 2%3 = 2, 3%3 = 0... 
            if ind_block < 0 or ind_block > (superblock.total_num_of_blocks - 1):
               if (count + 1) == 1: # level 1 -> 0%3 = 0 -> 0 + 1 = 1
                  invalid_print(1, ind_block, node.inode_num, offset_1)
               elif (count + 1) == 2: # level 2 -> 1%3 = 1 -> 1 + 1 = 2
                  invalid_print(2, ind_block, node.inode_num, offset_2)
               elif (count + 1) == 3: # level 3 -> 2%3 = 2 -> 2 + 1 = 3
                  invalid_print(3, ind_block, node.inode_num, offset_3)
            
            if ind_block < end_of_reserved and ind_block > 0:
               if (count + 1) == 1: # level 1
                  reserved_print(1, ind_block, node.inode_num, offset_1)
               elif (count + 1) == 2: # level 2:
                  reserved_print(2, ind_block, node.inode_num, offset_2)
               elif (count + 1) == 3: # level 3:
                  reserved_print(3, ind_block, node.inode_num, offset_3)
            
            if ind_block != 0 and ind_block > 0:
               if ind_block not in used_block:
                  if (count + 1) == 1: # level 1:
                     used_block[ind_block] = [[ind_block, node.inode_num, 1, offset_1]]
                  elif (count + 1) == 2: # level 2:
                     used_block[ind_block] = [[ind_block, node.inode_num, 2, offset_2]]
                  elif (count + 1) == 3: # level 3:
                     used_block[ind_block] = [[ind_block, node.inode_num, 3, offset_3]]
               else:
                  if (count + 1) == 1: # level 1:
                     used_block[ind_block].append([ind_block, node.inode_num, 1, offset_1])
                  elif (count + 1) == 2: # level 2:
                     used_block[ind_block].append([ind_block, node.inode_num, 2, offset_2])
                  elif (count + 1) == 3: # level 3:
                     used_block[ind_block].append([ind_block, node.inode_num, 3, offset_3])
            count += 1
   
   for ind_blk in indirect_list:
      #print(ind_blk.block_num_of_referenced_block)
      ind_offset = None
      if ind_blk.level == 1:
         ind_offset = offset_1
      elif ind_blk.level == 2:
         ind_offset = offset_2
      elif ind_blk.level == 3:
         ind_offset = offset_3

      if ind_blk.block_num_of_referenced_block < 0 or ind_blk.block_num_of_referenced_block > (superblock.total_num_of_blocks - 1):
         invalid_print(ind_blk.level, ind_blk.block_num_of_referenced_block, ind_blk.inode_num_owning_file, ind_offset)

      if ind_blk.block_num_of_referenced_block < end_of_reserved and ind_blk.block_num_of_referenced_block > 0:
         reserved_print(ind_blk.level, ind_block.block_num_of_referenced_block, ind_blk.inode_num_owning_file, ind_offset)

      if ind_blk.block_num_of_referenced_block != 0 and ind_blk.block_num_of_referenced_block > 0:
         if ind_blk.block_num_of_referenced_block not in used_block:
            used_block[ind_blk.block_num_of_referenced_block] = [[ind_blk.block_num_of_referenced_block, ind_blk.inode_num_owning_file, ind_blk.level, ind_offset]]
         else:
            used_block[ind_blk.block_num_of_referenced_block].append([ind_blk.block_num_of_referenced_block, ind_blk.inode_num_owning_file, ind_blk.level, ind_offset])

   #now we iterate over the data blocks from the end of the reserved blocks all the way until the beginning of the next group block
  # print("Free list start")
  # for j in bfree_list:
     # print(j.free_block_num)
   
   #print("blocks start")

   read_bfree = []
   for k in bfree_list:
      read_bfree.append(k.free_block_num)

   for blk in range(end_of_reserved, superblock.total_num_of_blocks):
      if blk in used_block: #checks to see if block is allocated
         if blk in read_bfree: #checks to see if block is in the free list
            allocated_print( blk )
         #print("The length of {} is {}".format(used_block[blk], len(used_block[blk])))
         if len(used_block[blk]) > 1: #this is basically telling us if there are duplicates or not
            #print("The length is {}".format(used_block[blk]))
            x = 0
            while x < len(used_block[blk]):
               dup_print(used_block[blk][x][0], used_block[blk][x][1], used_block[blk][x][2], used_block[blk][x][3])
              # print(used_block[blk][x][0])
               x += 1

      elif blk not in read_bfree: #might have to change elif to if and add in and blk not in used_block
         print("UNREFERENCED BLOCK {0}".format(blk))
         
 
   
   
   
         


def print_inode_consistency():
   #external list for keeping track of the inode block values that are allocated

   alloc_inodes = []
   read_free = []
  
   #print("This is free list: ")
   global inconsistencies


   for x in ifree_list:
      read_free.append(x.free_inode_num)
  
   #for l in read_free:
    #  print(l)
   #print("This is alloc list: ")
   #for y in inode_list:
     # print(y.inode_num)
   #print("End")


   for inode in inode_list:
      if inode.file_type != '0': # if file type is 0, then it's not allocated
         alloc_inodes.append(inode.inode_num)
         #print("This is the inode num {}".format(inode.inode_num))
         if inode.inode_num in read_free:
            inconsistencies = True
            print("ALLOCATED INODE {0} ON FREELIST".format(inode.inode_num))
      elif inode.file_type == '0':
         if inode.inode_num not in read_free:
            inconsistencies = True
            print("UNALLOCATED INODE {0} NOT ON FREELIST".format(inode.inode_num))
     
   for inode in range(superblock.first_nonreserved_inode, superblock.total_num_of_inodes):
      if inode not in alloc_inodes and inode not in read_free:
         inconsistencies = True
         print("UNALLOCATED INODE {0} NOT ON FREELIST".format(inode))
            

def invalid_dir_print(dir_inode, name, ref_inode):
   global inconsistencies
   inconsistencies = True
   print("DIRECTORY INODE {0} NAME {1} INVALID INODE {2}".format(dir_inode, name, ref_inode))

def unalloc_dir_print(dir_inode, name, ref_inode):
   global inconsistencies
   inconsistencies = True
   print("DIRECTORY INODE {0} NAME {1} UNALLOCATED INODE {2}".format(dir_inode, name, ref_inode))

def bad_link_ct_print(inode_num, reported_link_ct, link_ct):
   global inconsistencies
   inconsistencies = True
   print("INODE {0} HAS {1} LINKS BUT LINKCOUNT IS {2}".format(inode_num, reported_link_ct, link_ct))

def name_link_inconst_print(parent_num, name, ref_file_inode_num):
   global inconsistencies
   inconsistencies = True
   if name == "'.'":
      print("DIRECTORY INODE {0} NAME {1} LINK TO INODE {2} SHOULD BE {3}".format(parent_num, name, ref_file_inode_num, parent_num))
   elif name == "'..'":
      print("DIRECTORY INODE {0} NAME {1} LINK TO INODE {2} SHOULD BE {3}".format(parent_num, name, ref_file_inode_num, parent_num))
   
def print_direct_consistency():
   #https://stackoverflow.com/questions/9139897/how-to-set-default-value-to-all-keys-of-a-dict-object-in-python
   #needed to default all the values in the dictionary for link_ct to 0 because a KeyError was being given because I was trying to add 1 to a value that didn't exist
   parent_inode = {2:2}
   link_ct = defaultdict(lambda: 0)
   read_inode_val = [] #gonna be used to get the numerical values for the inode number from inode_list
   #create a dictionary for the different link count values to reference
   good_dir = False
   for y in inode_list:
      read_inode_val.append(y.inode_num)

   for dir in dirent_list:
      if dir.ref_file_inode_num < 1 or dir.ref_file_inode_num > superblock.total_num_of_inodes:
         invalid_dir_print(dir.parent_num, dir.name, dir.ref_file_inode_num)
      elif dir.ref_file_inode_num not in read_inode_val:
         unalloc_dir_print(dir.parent_num, dir.name, dir.ref_file_inode_num)
      else:
         good_dir = True
      if good_dir:
         link_ct[dir.ref_file_inode_num] += 1
         if dir.name != "'.'" and dir.name != "'..'":
            parent_inode[dir.ref_file_inode_num] = dir.parent_num
   
   for inode in inode_list:
      if link_ct[inode.inode_num] != inode.link_ct:
         bad_link_ct_print(inode.inode_num, link_ct[inode.inode_num], inode.link_ct)

   for dirent in dirent_list:
      if dirent.name == "'.'" and dirent.ref_file_inode_num != dirent.parent_num:
         name_link_inconst_print(dirent.parent_num, dirent.name, dirent.ref_file_inode_num)
      if dirent.name == "'..'" and dirent.ref_file_inode_num != parent_inode[dirent.parent_num]:
         name_link_inconst_print(dirent.parent_num, dirent.name, dirent.ref_file_inode_num)
      
def main():
    if len(sys.argv) != 2:
        print("Error: Command Line Aguments Input Incorrectly. Correct Form: lab3b [CSV file]", file=sys.stderr)
        sys.exit(1)
    read_csv( sys.argv[1] )
   # print(superblock)
   # print(group)
    #print(superblock.total_num_of_blocks)
    print_block_consistency()
    print_inode_consistency()
    print_direct_consistency()
    global inconsistencies
    if inconsistencies:
       #print("test exit 2")
       exit(2) #successful execution, inconsistencies found.
    else:
       #print("test exit 0")
       exit(0) #successful execution, no inconsistencies found.
    
if __name__ == '__main__':
    main()
