# Use this file at your own risk.
# Always ensure that the required files are actually put into the archive,
# and that no additional files are archived.
# You can use the zipinfo command to check the file structure of an existing archive.

USERNAME = csXXXXXX
EXERCISE = 02

# This excludes some common directories automatically.
# Adjust this list according to your setup.
# The zip command will also show which files are put into the archive.
# Check this output to ensure that only the required files are included.
EXCLUDE_PATTERNS = "**.vscode/*" "**.idea/*" "**__MACOSX/*" "**.DS_Store/*" "**.dSYM/*"

ARCHIVE= "../exc$(EXERCISE)_$(USERNAME).zip"

.PHONY: all
all: zip

.PHONY: zip
zip: clean
	$(RM) $(ARCHIVE)
	zip -r $(ARCHIVE) . --exclude $(EXCLUDE_PATTERNS)

.PHONY: clean
clean:
	@for dir in task?; do \
	  $(MAKE) -C "$$dir" clean; \
	done
