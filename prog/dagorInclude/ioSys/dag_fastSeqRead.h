//
// Dagor Engine 6.5
// Copyright (C) 2023  Gaijin Games KFT.  All rights reserved
// (for conditions of use see prog/license.txt)
//
#pragma once

#include <ioSys/dag_baseIo.h>
#include <util/dag_simpleString.h>

#include <supp/dag_define_COREIMP.h>

//
// Async file reader for sequental data reading; designed for parallel data loading and processing
//
class FastSeqReader : public IBaseLoad
{
public:
#if _TARGET_C3

#elif _TARGET_ANDROID
  // Block size can be different for every device, but with buffer 8192 bytes, the render is more smoothly
  // also Android java classes uses this value as default (DEFAULT_BUFFER_SIZE = 8192)
  enum {BUF_CNT = 6, BUF_SZ = 8 << 10, BUF_ALL_MASK = (1 << BUF_CNT) - 1, BLOCK_SIZE = 8 << 10};
#else
  enum
  {
    BUF_CNT = 6,
    BUF_SZ = 96 << 10,
    BUF_ALL_MASK = (1 << BUF_CNT) - 1,
    BLOCK_SIZE = 32 << 10
  };
#endif
  struct Range
  {
    int start, end;
  };

public:
  KRNLIMP FastSeqReader();
  ~FastSeqReader() { closeData(); }

  virtual void read(void *ptr, int size)
  {
    if (tryRead(ptr, size) != size)
      DAGOR_THROW(LoadException("read error", file.pos));
  }
  KRNLIMP virtual int tryRead(void *ptr, int size);
  virtual int tell() { return file.pos; }
  KRNLIMP virtual void seekto(int pos);
  virtual void seekrel(int ofs) { seekto(file.pos + ofs); }
  KRNLIMP virtual const char *getTargetName() { return targetFilename.str(); }
  KRNLIMP int64_t getTargetDataSize() override { return file.size; }

  //! assigns async file handle to read data from
  KRNLIMP void assignFile(void *handle, unsigned base_ofs, int size, const char *tgt_name, int min_chunk_size, int max_back_seek = 0);

  //! assigns set of ranges from where data will be read; any read outside these areas results in lockup
  void setRangesOfInterest(dag::Span<Range> r) { ranges = r; }

  //! resets buffers and flushes pending read-op
  KRNLIMP void reset();

  //! wait for prebuffering done
  void waitForBuffersFull();

  KRNLIMP static const char *resolve_real_name(const char *fname, unsigned &out_base_ofs, unsigned &out_size, bool &out_non_cached);

protected:
  struct File
  {
    int pos;
    int size;
    void *handle;
    unsigned chunkSize;
    unsigned baseOfs;
  } file;

  struct Buf
  {
    int sa, ea;
    char *data;
    short mask, handle;
  } buf[BUF_CNT], *cBuf;

  unsigned pendMask, doneMask;
  int readAheadPos, lastSweepPos, maxBackSeek;
  dag::Span<Range> ranges;
  SimpleString targetFilename;
  int64_t curThreadId = -1;

  void waitForDone(int wait_mask);
  void placeRequests();
  KRNLIMP void closeData();
  void checkThreadSanity();
};


//
// Generic file read interface based on FastSeqReader
//
class FastSeqReadCB : public FastSeqReader
{
public:
  ~FastSeqReadCB() { close(); }

  KRNLIMP bool open(const char *fname, int max_back_seek = 0, const char *base_path = NULL);
  KRNLIMP void close();

  bool isOpen() const { return file.handle != NULL; }
  int getSize() const { return file.size; }

  void *getFileHandle() const { return file.handle; }

  SimpleString targetFilename;
};

#include <supp/dag_undef_COREIMP.h>
