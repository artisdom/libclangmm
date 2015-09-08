#include "CodeCompleteResults.h"
#include "CompletionString.h"
#include <exception>

clang::CodeCompleteResults::
CodeCompleteResults(CXTranslationUnit &cx_tu, const std::string &file_name,
                    const std::map<std::string, std::string>  &buffers,
                    unsigned line_num, unsigned column) {
  std::vector<CXUnsavedFile> files;
  for (auto &buffer : buffers) {
    CXUnsavedFile file;
    file.Filename = buffer.first.c_str();
    file.Contents = buffer.second.c_str();
    file.Length = buffer.second.size();
    files.push_back(file);
  }
  cx_results = clang_codeCompleteAt(cx_tu,
                                  file_name.c_str(),
                                  line_num,
                                  column,
                                  files.data(),
                                  files.size(),
                                  clang_defaultCodeCompleteOptions()|CXCodeComplete_IncludeBriefComments);
  if(cx_results!=NULL)
    clang_sortCodeCompletionResults(cx_results->Results, cx_results->NumResults);
}

clang::CodeCompleteResults::~CodeCompleteResults() {
  clang_disposeCodeCompleteResults(cx_results);
}

unsigned clang::CodeCompleteResults::
size() {
  if(cx_results==NULL)
    return 0;
  return cx_results->NumResults;
}

clang::CompletionString clang::CodeCompleteResults::
get(unsigned i) {
  if (i >= size()) {
    throw std::invalid_argument("clang::CodeCompleteResults::get(unsigned i): i>=size()");
  }
  return CompletionString(cx_results->Results[i].CompletionString);
}
