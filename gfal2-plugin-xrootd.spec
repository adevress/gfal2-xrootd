Name:           gfal2-plugin-xrootd
Version:        0.1.0
Release:        1%{?dist}
Summary:        Provide xrootd support for GFAL2

Group:          Applications/Internet
License:        ASL 2.0
URL:            https://svnweb.cern.ch/trac/lcgutil/wiki/gfal2
Source0:        https://dcameron.web.cern.ch/dcameron/dev/rpmbuild/SOURCES/%{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  glib2-devel
BuildRequires:  gfal2-devel
BuildRequires:  xrootd-devel

%description
The Grid File Access Library, GFAL2, provides a simple POSIX-like API for file
operations in grid and cloud environments. Plug-ins are available to allow
access via a variety of protocols. This package contains a plugin for the
xrootd protocol (root://).

%global pkgdir gfal2-plugins

%prep
%setup -q

%build
%configure
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

# Remove libtool .la files
find %{buildroot} -type f -name \*.la -exec rm -fv '{}' ';'

%clean
rm -rf %{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/%{pkgdir}/libgfal_plugin_xrootd.so
%{_docdir}/%{name}-%{version}/README

%changelog
* Tue May 22 2012 David Cameron <d.g.cameron@fys.uio.no> 0.1.0-1
- Initial version
