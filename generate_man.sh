#! /bin/sh
#
# Generate man pages from Markdown files.
# (`pandoc` is required.)
#
MD_FILES="gobb_analyze.1.md gobb_inspect.1.md"

for MD_FILE in ${MD_FILES}; do
    if [ ! -f "${MD_FILE}" ]; then
        echo "file not found: $MD_FILE" >&2
        continue
    fi

    #
    # Set variables.
    #
    MAN_FILE="$(printf '%s' "$MD_FILE" | sed -e 's|\.md$||')"
    TMP_MD_FILE="${MD_FILE}.$$"
    TMP_MAN_FILE="${MAN_FILE}.$$"
    trap "rm -f -- '$TMP_MD_FILE' '$TMP_MAN_FILE'; exit 1" 1 2 3 15
    rm -f -- "$TMP_MD_FILE" "$TMP_MAN_FILE"

    MAN_TITLE="$(basename -- "$MAN_FILE" \
        | sed -e 's|^\(.*\)\.[1-9]$|\1|' -e 'y|abcdefghijklmnopqrstuvwxyz|ABCDEFGHIJKLMNOPQRSTUVWXYZ|')"
    MAN_SECTION="$(basename -- "$MAN_FILE" | sed -e 's|^.*\.\([1-9]\)$|\1|')"
    AUTHOR="$(head -1 AUTHORS)"
    DATE="$(env LC_ALL= LC_TIME= LANG=C date +'%b %Y')"

    #
    # Insert a header to the Markdown file.
    # (Pandoc interpretes the header.)
    #
    (
        printf '%% %s(%s)\n' "$MAN_TITLE" "$MAN_SECTION"
        printf '%% %s\n' "$AUTHOR"
        printf '%% %s\n' "$DATE"
        echo
        cat -- "$MD_FILE"
    ) >> "$TMP_MD_FILE"

    #
    # Generate a man file.
    #
    if ! pandoc -s -t man "$TMP_MD_FILE" > "$TMP_MAN_FILE"; then
        echo "failed to generated / updated: $MAN_FILE" >&2
    elif ! cmp -- "$TMP_MAN_FILE" "$MAN_FILE" > /dev/null 2>&1; then
        mv -f -- "$TMP_MAN_FILE" "$MAN_FILE"
        echo "generated / updated: $MAN_FILE" >&2
    fi

    rm -f -- "$TMP_MD_FILE" "$TMP_MAN_FILE"
done
