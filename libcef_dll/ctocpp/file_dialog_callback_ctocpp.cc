// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//

#include "libcef_dll/ctocpp/file_dialog_callback_ctocpp.h"
#include "libcef_dll/transfer_util.h"


// VIRTUAL METHODS - Body may be edited by hand.

void CefFileDialogCallbackCToCpp::Continue(
    const std::vector<CefString>& file_paths) {
  if (CEF_MEMBER_MISSING(struct_, cont))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Translate param: file_paths; type: string_vec_byref_const
  cef_string_list_t file_pathsList = cef_string_list_alloc();
  DCHECK(file_pathsList);
  if (file_pathsList)
    transfer_string_list_contents(file_paths, file_pathsList);

  // Execute
  struct_->cont(struct_,
      file_pathsList);

  // Restore param:file_paths; type: string_vec_byref_const
  if (file_pathsList)
    cef_string_list_free(file_pathsList);
}

void CefFileDialogCallbackCToCpp::Cancel() {
  if (CEF_MEMBER_MISSING(struct_, cancel))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  struct_->cancel(struct_);
}


#ifndef NDEBUG
template<> long CefCToCpp<CefFileDialogCallbackCToCpp, CefFileDialogCallback,
    cef_file_dialog_callback_t>::DebugObjCt = 0;
#endif

