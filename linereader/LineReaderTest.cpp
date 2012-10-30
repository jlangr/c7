// Copyright (c) 2009, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "gmock/gmock.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "LineReader.h"

static int TemporaryFile() {
  static const char templ[] = "/tmp/line-reader-unittest-XXXXXX";
  char templ_copy[sizeof(templ)];
  memcpy(templ_copy, templ, sizeof(templ));
  const int fd = mkstemp(templ_copy);
  if (fd >= 0)
    unlink(templ_copy);

  return fd;
}

static int WriteTemporaryFile(const char* records) {
  const int fd = TemporaryFile();
  write(fd, records, strlen(records));
  lseek(fd, 0, SEEK_SET);
  return fd;
}

class LineReaderTest: public testing::Test {
public:
   int fd;
   const char *line;
   unsigned len;
//START:OneLine
void ASSERT_EQ_WITH_LENGTH(
   const char* expected, const char* actual, unsigned length) {
   ASSERT_EQ(length, strlen(actual));
   ASSERT_STREQ(expected, actual);
}
//END:OneLine
};

//namespace {
//typedef testing::Test LineReaderTest;
//}

TEST_F(LineReaderTest, EmptyFile) {
  fd = WriteTemporaryFile("");
  LineReader reader(fd);

  const char *line;
  unsigned len;
  ASSERT_FALSE(reader.GetNextLine(&line, &len));
}

TEST_F(LineReaderTest, OneLineTerminated) {
  int fd = WriteTemporaryFile("a\n");
  LineReader reader(fd);

  const char *line;
  unsigned int len;
  ASSERT_TRUE(reader.GetNextLine(&line, &len));
  ASSERT_EQ(len, (unsigned int)1);
  ASSERT_EQ(line[0], 'a');
  ASSERT_EQ(line[1], 0);
  reader.PopLine(len);

  ASSERT_FALSE(reader.GetNextLine(&line, &len));
}

// START:OneLine

TEST_F(LineReaderTest, OneLine) {
  LineReader reader(WriteTemporaryFile("a"));

  ASSERT_TRUE(reader.GetNextLine(&line, &len));
// START_HIGHLIGHT
  ASSERT_EQ(len, (unsigned)1);
  ASSERT_EQ(line[0], 'a');
  ASSERT_EQ(line[1], 0);
// END_HIGHLIGHT
  reader.PopLine(len);

  ASSERT_FALSE(reader.GetNextLine(&line, &len));
}
// END:OneLine

TEST_F(LineReaderTest, TwoLinesTerminated) {
  fd = WriteTemporaryFile("a\nb\n");
  LineReader reader(fd);

  const char *line;
  unsigned len;
  ASSERT_TRUE(reader.GetNextLine(&line, &len));
  ASSERT_EQ(len, (unsigned)1);
  ASSERT_EQ(line[0], 'a');
  ASSERT_EQ(line[1], 0);
  reader.PopLine(len);

  ASSERT_TRUE(reader.GetNextLine(&line, &len));
  ASSERT_EQ(len, (unsigned)1);
  ASSERT_EQ(line[0], 'b');
  ASSERT_EQ(line[1], 0);
  reader.PopLine(len);

  ASSERT_FALSE(reader.GetNextLine(&line, &len));
}

TEST_F(LineReaderTest, TwoLines) {
  fd = WriteTemporaryFile("a\nb");
  LineReader reader(fd);

  const char *line;
  unsigned len;
  ASSERT_TRUE(reader.GetNextLine(&line, &len));
  ASSERT_EQ(len, (unsigned)1);
  ASSERT_EQ(line[0], 'a');
  ASSERT_EQ(line[1], 0);
  reader.PopLine(len);

  ASSERT_TRUE(reader.GetNextLine(&line, &len));
  ASSERT_EQ(len, (unsigned)1);
  ASSERT_EQ(line[0], 'b');
  ASSERT_EQ(line[1], 0);
  reader.PopLine(len);

  ASSERT_FALSE(reader.GetNextLine(&line, &len));
}

TEST_F(LineReaderTest, MaxLength) {
  char l[LineReader::kMaxLineLen - 1];
  memset(l, 'a', sizeof(l));
  fd = WriteTemporaryFile(l);
  LineReader reader(fd);

  const char *line;
  unsigned len;
  ASSERT_TRUE(reader.GetNextLine(&line, &len));
  ASSERT_EQ(len, sizeof(l));
  ASSERT_TRUE(memcmp(l, line, sizeof(l)) == 0);
  ASSERT_EQ(line[len], 0);
}

TEST_F(LineReaderTest, TooLong) {
  char l[LineReader::kMaxLineLen];
  memset(l, 'a', sizeof(l));
  fd = WriteTemporaryFile(l);
  LineReader reader(fd);

  const char *line;
  unsigned len;
  ASSERT_FALSE(reader.GetNextLine(&line, &len));
}

