#include <stdlib.h>
#include <glib.h>
#include <clang-c/Index.h>

static enum CXChildVisitResult
visitor_cb (CXCursor     cursor,
	    CXCursor     parent,
	    CXClientData user_data)
{
	CXSourceLocation location;
	enum CXCursorKind kind;
	CXString kind_string;
	gint *level = user_data;
	gchar *level_str;

	location = clang_getCursorLocation (cursor);
	if (!clang_Location_isFromMainFile (location))
	{
		return CXChildVisit_Continue;
	}

	kind = clang_getCursorKind (cursor);
	kind_string = clang_getCursorKindSpelling (kind);

	level_str = g_strnfill (*level * 2, ' ');

	g_print ("%s%s\n",
		 level_str,
		 clang_getCString (kind_string));

	(*level)++;
	clang_visitChildren (cursor, visitor_cb, level);
	(*level)--;

	return CXChildVisit_Continue;
}

int
main (int    argc,
      char **argv)
{
	CXIndex index;
	CXTranslationUnit translation_unit;
	CXCursor root_cursor;
	gint level;

	if (argc != 2)
	{
		g_printerr ("Usage: %s <file.c>\n", argv[0]);
		return EXIT_FAILURE;
	}

	index = clang_createIndex (0, 1);
	translation_unit = clang_createTranslationUnitFromSourceFile (index,
								      argv[1],
								      0, NULL,
								      0, NULL);

	root_cursor = clang_getTranslationUnitCursor (translation_unit);

	level = 0;
	clang_visitChildren (root_cursor, visitor_cb, &level);

	clang_disposeTranslationUnit (translation_unit);
	clang_disposeIndex (index);

	return EXIT_SUCCESS;
}
