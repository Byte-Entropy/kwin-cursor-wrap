# Maintainer: Javier Andres Dal Monte Casoni <Byte-Entropy>
pkgname=kwin-cursor-wrap
pkgver=1.0.0
pkgrel=1
pkgdesc="Horizontal monitor edge cursor wrapping for KDE Plasma 6 Wayland"
arch=('x86_64')
url="https://github.com/Byte-Entropy/kwin-cursor-wrap"
license=('MIT')
depends=('systemd-libs' 'kwin')
makedepends=('cmake' 'git')
install="${pkgname}.install"
# For local testing; swap "${url}.git#tag=v${pkgver}" in when publishing to AUR
source=("${pkgname}::git+file://${PWD}")
sha256sums=('SKIP')

build() {
    cmake -B build -S "${pkgname}" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -Wno-dev
    cmake --build build
}

package() {
    DESTDIR="${pkgdir}" cmake --install build
    install -Dm644 "${pkgname}/LICENSE" "${pkgdir}/usr/share/licenses/${pkgname}/LICENSE"
}

