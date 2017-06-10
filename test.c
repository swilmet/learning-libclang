#include <stdlib.h>
#include <glib.h>
#include <clang-c/Index.h>

typedef struct
{
	CXTranslationUnit translation_unit;
	gint level;
} Data;

static enum CXChildVisitResult
visitor_cb (CXCursor     cursor,
	    CXCursor     parent,
	    CXClientData user_data)
{
	CXSourceLocation location;
	enum CXCursorKind kind;
	CXString kind_string;
	Data *data = user_data;
	gchar *level_str;

	location = clang_getCursorLocation (cursor);
	if (!clang_Location_isFromMainFile (location))
	{
		return CXChildVisit_Continue;
	}

	kind = clang_getCursorKind (cursor);
	kind_string = clang_getCursorKindSpelling (kind);

	level_str = g_strnfill (data->level * 2, ' ');

	g_print ("%s%s",
		 level_str,
		 clang_getCString (kind_string));

	clang_disposeString (kind_string);

	if (kind == CXCursor_FunctionDecl)
	{
		CXSourceRange range;
		CXSourceLocation start;
		CXSourceLocation end;
		unsigned line;
		unsigned column;
		unsigned offset;

		range = clang_getCursorExtent (cursor);
		start = clang_getRangeStart (range);
		end = clang_getRangeEnd (range);

		clang_getSpellingLocation (start, NULL, &line, &column, &offset);
		g_print (": [line=%u, col=%u, offset=%u]",
			 line, column, offset);

		clang_getSpellingLocation (end, NULL, &line, &column, &offset);
		g_print (" -> [line=%u, col=%u, offset=%u]",
			 line, column, offset);
	}

	g_print ("\n");

	data->level++;
	clang_visitChildren (cursor, visitor_cb, data);
	data->level--;

	return CXChildVisit_Continue;
}

int
main (int    argc,
      char **argv)
{
	CXIndex index;
	CXCursor root_cursor;
	Data data;

	if (argc != 2)
	{
		g_printerr ("Usage: %s <file.c>\n", argv[0]);
		return EXIT_FAILURE;
	}

	index = clang_createIndex (0, 1);
	data.translation_unit = clang_createTranslationUnitFromSourceFile (index,
									   argv[1],
									   0, NULL,
									   0, NULL);

	root_cursor = clang_getTranslationUnitCursor (data.translation_unit);

	data.level = 0;
	clang_visitChildren (root_cursor, visitor_cb, &data);

	clang_disposeTranslationUnit (data.translation_unit);
	clang_disposeIndex (index);

	return EXIT_SUCCESS;
}
