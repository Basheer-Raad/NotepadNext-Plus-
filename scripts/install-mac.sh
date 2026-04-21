#!/bin/bash
set -euo pipefail

# -------------------------------------------------------------------
# install-mac.sh — Build and install NotepadNext variants on macOS
#
# Usage:
#   ./scripts/install-mac.sh original          # dry-run (default)
#   ./scripts/install-mac.sh plus              # dry-run (default)
#   ./scripts/install-mac.sh original --install # actually build + install
#   ./scripts/install-mac.sh plus --install     # actually build + install
# -------------------------------------------------------------------

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

VARIANT="${1:-}"
INSTALL_FLAG="${2:-}"

if [[ "$VARIANT" != "original" && "$VARIANT" != "plus" ]]; then
    echo "Usage: $0 <original|plus> [--install]"
    echo ""
    echo "  original  — Build upstream NotepadNext (green icon)"
    echo "  plus      — Build enhanced NotepadNextPlus (blue icon)"
    echo ""
    echo "Options:"
    echo "  --install — Actually build and install. Without this flag,"
    echo "              the script only shows what it would do (dry run)."
    exit 1
fi

if [[ "$VARIANT" == "plus" ]]; then
    BUILD_DIR="$REPO_ROOT/build-plus"
    CMAKE_FLAG="-DNOTEPAD_PLUS_BUILD=ON"
    APP_NAME="NotepadNextPlus"
    DISPLAY_NAME="Notepad Next Plus"
else
    BUILD_DIR="$REPO_ROOT/build-original"
    CMAKE_FLAG="-DNOTEPAD_PLUS_BUILD=OFF"
    APP_NAME="NotepadNext"
    DISPLAY_NAME="Notepad Next"
fi

INSTALL_DIR="$BUILD_DIR/install"
APP_BUNDLE="$INSTALL_DIR/$APP_NAME.app"
DEST="/Applications/$APP_NAME.app"

echo "=== NotepadNext Mac Installer ==="
echo "Variant:       $DISPLAY_NAME"
echo "Build dir:     $BUILD_DIR"
echo "CMake flag:    $CMAKE_FLAG"
echo "App bundle:    $APP_BUNDLE"
echo "Install to:    $DEST"
echo ""

if [[ "$INSTALL_FLAG" != "--install" ]]; then
    echo "[DRY RUN] Would run the following steps:"
    echo ""
    echo "  1. cmake -S \"$REPO_ROOT\" -B \"$BUILD_DIR\" $CMAKE_FLAG"
    echo "  2. cmake --build \"$BUILD_DIR\" --parallel"
    echo "  3. cmake --build \"$BUILD_DIR\" --target install_local"
    echo "  4. macdeployqt \"$APP_BUNDLE\""
    echo "  5. cp -R \"$APP_BUNDLE\" \"$DEST\""
    echo ""
    echo "To execute for real, re-run with --install:"
    echo "  $0 $VARIANT --install"
    exit 0
fi

echo "Step 1/5: Configuring CMake..."
cmake -S "$REPO_ROOT" -B "$BUILD_DIR" $CMAKE_FLAG

echo ""
echo "Step 2/5: Building..."
cmake --build "$BUILD_DIR" --parallel

echo ""
echo "Step 3/5: Installing to staging directory..."
cmake --build "$BUILD_DIR" --target install_local

echo ""
echo "Step 4/5: Running macdeployqt (bundling Qt frameworks)..."
MACDEPLOYQT="$(command -v macdeployqt 2>/dev/null || true)"
if [[ -z "$MACDEPLOYQT" ]]; then
    echo "ERROR: macdeployqt not found in PATH."
    echo "Make sure Qt's bin directory is in your PATH, e.g.:"
    echo "  export PATH=\"\$(brew --prefix qt)/bin:\$PATH\""
    exit 1
fi
"$MACDEPLOYQT" "$APP_BUNDLE"

echo ""
echo "Step 5/5: Copying to /Applications..."
if [[ -d "$DEST" ]]; then
    echo "  Removing existing $DEST"
    rm -r "$DEST"
fi
cp -R "$APP_BUNDLE" "$DEST"

echo ""
echo "Done! Installed $DISPLAY_NAME to $DEST"
echo ""
echo "NOTE: On first launch, macOS Gatekeeper may block the app because"
echo "it is unsigned. To fix this, either:"
echo "  - Right-click the app → Open → click Open in the dialog, OR"
echo "  - Run: xattr -cr \"$DEST\""
