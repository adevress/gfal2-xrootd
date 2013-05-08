Name:           gfal2-plugin-xrootd
Version:        0.2.2
Release:        1%{?dist}
Summary:        Provide xrootd support for GFAL2

Group:          Applications/Internet
License:        ASL 2.0
URL:            https://svnweb.cern.ch/trac/lcgutil/wiki/gfal2
Source0:        http://grid-deployment.web.cern.ch/grid-deployment/dms/lcgutil/tar/%{name}/%{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  cmake
BuildRequires:  gfal2-devel
BuildRequires:  xrootd-client-devel

%description
The Grid File Access Library, GFAL2, provides a simple POSIX-like API for file
operations in grid and cloud environments. Plug-ins are available to allow
access via a variety of protocols. This package contains a plugin for the
xrootd protocol (root://).

%global pkgdir gfal2-plugins

%prep
%setup -q

%build
%cmake \
-DCMAKE_INSTALL_PREFIX=/ .
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

%clean
rm -rf %{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/%{pkgdir}/libgfal_plugin_xrootd.so
%{_docdir}/%{name}-%{version}/*

%changelog
* Mon Apr 29 2013 Adrien Devresse <adevress at cern.ch> - 0.2.2-1
 - support for 3rd party copy with xrootd
 - correct compilation problem with 32 bits
 - correct EL5 compilation problem
 - Normalize spec file for EPEL
 - switch the copyright ownership to EMI

* Thu Mar 21 2013 Alejandro Alvarez <aalvarez@cern.ch> - 0.1.1-2
- Update to compile with xrootd 3.3.1

* Tue Jan 29 2013 Alejandro Alvarez <aalvarez@cern.ch> - 0.1.1-1
- All URL's are treated as full path

* Fri Jan 18 2013 Alejandro Alvarez <aalvarez@cern.ch> - 0.1.0-2
- Using CMake

* Tue May 22 2012 David Cameron <d.g.cameron@fys.uio.no> - 0.1.0-1
- Initial version

