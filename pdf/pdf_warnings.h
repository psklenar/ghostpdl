/* Copyright (C) 2022-2023 Artifex Software, Inc.
   All Rights Reserved.

   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied,
   modified or distributed except as expressly authorized under the terms
   of the license contained in the file LICENSE in this distribution.

   Refer to licensing information at http://www.artifex.com or contact
   Artifex Software, Inc.,  39 Mesa Street, Suite 108A, San Francisco,
   CA 94129, USA, for further information.
*/

#ifndef PARAM
#define PARAM(A,B) A
#endif
PARAM(W_PDF_NOWARNING,	            "no warning"),
PARAM(W_PDF_BAD_XREF_SIZE,          "incorrect xref size"),
PARAM(W_PDF_BAD_XREF_ENTRY_SIZE,    "xref entry not exactly 20 bytes"),
PARAM(W_PDF_BAD_XREF_ENTRY_NO_EOL,  "xref entry not terminated with EOL"),
PARAM(W_PDF_BAD_XREF_ENTRY_FORMAT,  "xref entry not valid format"),
PARAM(W_PDF_BAD_INLINEFILTER,       "used inline filter name inappropriately"),
PARAM(W_PDF_BAD_INLINECOLORSPACE,   "used inline colour space inappropriately"),
PARAM(W_PDF_BAD_INLINEIMAGEKEY,     "used inline image key inappropriately"),
PARAM(W_PDF_IMAGE_ERROR,            "recoverable image error"),
PARAM(W_PDF_BAD_IMAGEDICT,          "recoverable error in image dictionary"),
PARAM(W_PDF_BAD_IMAGENAME,          "An image dictionary has a /Name key whose value is not a name object"),
PARAM(W_PDF_TOOMANYQ,               "encountered more Q than q"),
PARAM(W_PDF_TOOMANYq,               "encountered more q than Q"),
PARAM(W_PDF_STACKGARBAGE,           "garbage left on stack"),
PARAM(W_PDF_STACKUNDERFLOW,         "stack underflow"),
PARAM(W_PDF_GROUPERROR,             "error in group definition"),
PARAM(W_PDF_OPINVALIDINTEXT,        "invalid operator used in text block"),
PARAM(W_PDF_NOTINCHARPROC,          "used invalid operator in CharProc"),
PARAM(W_PDF_NESTEDTEXTBLOCK,        "BT found inside a text block"),
PARAM(W_PDF_ETNOTEXTBLOCK,          "ET found outside text block"),
PARAM(W_PDF_TEXTOPNOBT,             "text operator outside text block"),
PARAM(W_PDF_DEGENERATETM,           "degenerate text matrix"),
PARAM(W_PDF_BADICC_USE_ALT,         "Invalid ICC colour profile, using alternate"),
PARAM(W_PDF_BADICC_USECOMPS,        "Invalid ICC colour profile, no (or bad) Alternate specified, using /N to select a device space"),
PARAM(W_PDF_BADTRSWITCH,            "bad value for text rendering mode"),
PARAM(W_PDF_BADSHADING,             "error in shading"),
PARAM(W_PDF_BADPATTERN,             "error in pattern"),
PARAM(W_PDF_NONSTANDARD_OP,         "non standard operator found - ignoring"),
PARAM(W_PDF_NUM_EXPONENT,           "number uses illegal exponent form"),
PARAM(W_PDF_STREAM_HAS_CONTENTS,    "Stream has inappropriate /Contents entry"),
PARAM(W_PDF_STREAM_BAD_DECODEPARMS, "bad DecodeParms"),
PARAM(W_PDF_STREAM_BAD_KEYWORD,     "A stream keyword was not terminated with a linefeed (0x0A)"),
PARAM(W_PDF_MASK_ERROR,             "error in Mask"),
PARAM(W_PDF_ANNOT_AP_ERROR,         "error in annotation Appearance"),
PARAM(W_PDF_BAD_NAME_ESCAPE,        "badly escaped name"),
PARAM(W_PDF_TYPECHECK,              "typecheck error"),
PARAM(W_PDF_BAD_TRAILER,            "bad trailer dictionary"),
PARAM(W_PDF_ANNOT_ERROR,            "error in annotation"),
PARAM(W_PDF_BAD_ICC_PROFILE_LINK,   "failed to create ICC profile link"),
PARAM(W_PDF_OVERFLOW_REAL,          "overflowed a real reading a number, assuming 0"),
PARAM(W_PDF_INVALID_REAL,           "failed to read a valid number, assuming 0"),
PARAM(W_PDF_DEVICEN_USES_ALL,       "A DeviceN space used the /All ink name."),
PARAM(W_PDF_BAD_MEDIABOX,           "Couldn't retrieve MediaBox for page, using current media size"),
PARAM(W_PDF_CA_OUTOFRANGE,          "CA or ca value not in range 0.0 to 1.0, clamped to range."),
PARAM(W_PDF_INVALID_DEFAULTSPACE,   "Invalid DefaultGray, DefaultRGB or DefaultCMYK space specified, ignored."),
PARAM(W_PDF_INVALID_DECRYPT_LEN,    "Invalid /Length supplied in Encryption dictionary."),
PARAM(W_PDF_INVALID_FONT_BASEENC,   "Ignoring invalid BaseEncoding name in font"),
PARAM(W_PDF_GROUP_HAS_COLORSPACE,   "Group attributes dictionary has /ColorSpace instead of /CS"),
PARAM(W_PDF_GROUP_BAD_BC,           "Group attributes dictionary /BC differs in number of components from the colour space"),
PARAM(W_PDF_INT_AS_REAL,            "found real number when expecting int"),
PARAM(W_PDF_NO_TREE_LIMITS,         "Name tree node missing required Limits entry"),
PARAM(W_PDF_BAD_TREE_LIMITS,        "Name tree node Limits array does not have 2 entries"),
PARAM(W_PDF_NAMES_ARRAY_SIZE,       "Name tree Names array size not a mulitple of 2"),
PARAM(W_PDF_MISSING_NAMED_RESOURCE, "Couldn't find a named resource"),
PARAM(W_PDF_BAD_OUTLINES,           "A problem was encountered trying to preserve the Outlines"),
PARAM(W_PDF_BAD_INFO,               "The file has a bad /Info dictionary"),
PARAM(W_PDF_BAD_EMBEDDEDFILES,      "File has Embedded files which could not be preserved"),
PARAM(W_PDF_BAD_ACROFORM,           "Bad AcroForm detected"),
PARAM(W_PDF_BAD_OUTPUTINTENTS,      "Bad OutputIntents detected"),
PARAM(W_PDF_BAD_PAGELABELS,         "A problem was encountered trying to preserve the page Labels"),
PARAM(W_PDF_XREF_OBJECT0_NOT_FREE,  "Xref entry for object 0 not a free entry."),
PARAM(W_PDF_ZEROSCALE_ANNOT,        "Annotation has a scale factor of zero and was ignored"),
PARAM(W_PDF_SMASK_MISSING_S,        "An SMask is missing the Subtype (/S) key, assuming Luminosity"),
PARAM(W_PDF_SMASK_UNKNOWN_S,        "An SMask has an unknown value for the Subtype (/S) key, assuming Luminosity"),
PARAM(W_PDF_SMASK_INVALID_TR,       "SMask had a transfer function (/TR) with number of inputs or outputs not 1"),
PARAM(W_PDF_SMASK_UNKNOWN_TR,       "SMask had a named transfer function (/TR) with an unknown name (not /Identity)"),
PARAM(W_PDF_SMASK_UNKNOWN_TR_TYPE,  "SMask had a transfer function (/TR) of the wrong object type"),
PARAM(W_PDF_SMASK_UNKNOWN_TYPE,     "SMask had an unknwon Type (not /Mask)"),
PARAM(W_PDF_MISSING_WHITE_OPS,      "Possible missing white space between operators"),
PARAM(W_PDF_ANNOT_BAD_TYPE,         "Annotation is not a dictionary, ignoring"),
PARAM(W_PDF_VERSION_NO_EOL,         "PDF version number not immediately followed with EOL"),
PARAM(W_PDF_D1_COLOUR_OP,           "A type 3 glyph description, beginning with d1, tried to set a colour"),
PARAM(W_PDF_FORM_CLIPPEDOUT,        "A Form XObject had a BBox with a width or height of 0"),
PARAM(W_PDF_FDESC_BAD_FONTNAME,     "A FontDescriptor has a missing or bad /FontName"),
PARAM(W_PDF_GARBAGE_B4HDR,          "File has some garbage before %PDF-"),
PARAM(W_PDF_FONTRESOURCE_TYPE,      "A Font key in a Resources dictionary has a value of the wrong type"),
PARAM(W_PDF_UNBLANACED_BT,          "A page ended after a BT had been executed and without a mtching ET"),
PARAM(W_PDF_MISMATCH_GENERATION,    "The generation number of an indirectly referenced object did not match the xref"),
PARAM(W_PDF_BAD_RENDERINGINTENT,    "A ri or /RI used an unknown named rendering intent"),
PARAM(W_PDF_BAD_VIEW,               "Couldn't read the initial document view"),
PARAM(W_PDF_BAD_WMODE,              "A Font or CMap has a WMode which is neither 0 (horizontal) nor 1 (vertical)"),
#undef PARAM
