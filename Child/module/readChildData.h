#include <stddef.h>

typedef enum{ CantOpen=0, FileBad } BadFileState_t;

class BadFile {
  BadFile& operator=(const BadFile&);

  char *filename_;
  BadFileState_t state;
 public:
  BadFile(BadFileState_t, const char*);
  BadFile(const BadFile&);
  ~BadFile();
  const char *filename() const;
  const char *ExceptionName() const;
};

class ReadChildData {
 private:
  ReadChildData(const ReadChildData&);
  ReadChildData& operator=(const ReadChildData&);
 protected:
  bool ReadNodes(const char *basename, int nStep);
  bool ReadTriangles(const char *basename, int nStep);
  bool ReadZ(const char *basename, int nStep, bool isArea);
  void AllocateNodes();
  void AllocateTriangles();
  void AllocateZ();

  size_t nnodes_;
  size_t ntri_;
  double currentTime_;   // time
 public:
  ReadChildData();
  ~ReadChildData();

  bool LoadData(const char *basename, int nStep, int TypeVariable);

  size_t nnodes() const { return nnodes_; }
  size_t ntri() const { return ntri_; }
  double currentTime() const { return currentTime_; }
  // We should use std::vector really but we need to cope with
  // non conformant compilers.
  // nodes
  float *x;
  float *y;
  // connectivity element/nodes
  int *p0;
  int *p1;
  int *p2;
  // elevation
  float *z;
};
