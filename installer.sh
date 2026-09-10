#!/usr/bin/env bash
set -euo pipefail
root=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
archive="$root/jester.zip"
[ -f "$archive" ] || { echo "installer: jester.zip not found; run 'make package' first" >&2; exit 1; }
tmp=$(mktemp -d); trap 'rm -rf "$tmp"' EXIT
unzip -q "$archive" -d "$tmp"
mkdir -p "$HOME/.jester" "$HOME/.jesterlibs" "$HOME/.jester/plugins"
install -m 0755 "$tmp/jester" "$HOME/.jester/jester-core"
rm -rf "$HOME/.jester/builtins"
cp -R "$tmp/builtins" "$HOME/.jester/builtins"
rm -f "$HOME/.jester/plugins/"*.so
for source in "$HOME/.jester/builtins/"*.c; do
  [ -e "$source" ] || continue
  name=$(basename "${source%.c}")
  ${CC:-cc} -std=c11 -O2 -fPIC -shared -o "$HOME/.jester/plugins/$name.so" "$source"
done
cat > "$HOME/.jester/jester" <<'WRAPPER'
#!/usr/bin/env bash
exec env JESTER_BUILTINS="$HOME/.jester/plugins" "$HOME/.jester/jester-core" "$@"
WRAPPER
chmod 0755 "$HOME/.jester/jester"
cp -Rn "$tmp/.jesterlibs_template/"* "$HOME/.jesterlibs/" 2>/dev/null || true
line='export PATH="$HOME/.jester:$PATH"'
for rc in "$HOME/.bashrc" "$HOME/.zshrc"; do
  [ -e "$rc" ] || continue
  grep -Fqx "$line" "$rc" || printf '\n%s\n' "$line" >> "$rc"
done
echo "Installed Jester. Restart your shell or run: export PATH=\"\$HOME/.jester:\$PATH\""
