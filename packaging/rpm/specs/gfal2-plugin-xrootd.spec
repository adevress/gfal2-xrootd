# unversionned doc dir F20 change https://fedoraproject.org/wiki/Changes/UnversionedDocdirs
%{!?_pkgdocdir: %global _pkgdocdir %{_docdir}/%{name}-%{version}}

Name:           gfal2-plugin-xrootd
Version:        0.2.2
Release:        3%{?dist}
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
-DCMAKE_INSTALL_PREFIX=/ \
-DDOC_INSTALL_DIR=%{_pkgdocdir} \
 . 

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
%config(noreplace) %{_sysconfdir}/gfal2.d/xrootd_plugin.conf
%{_libdir}/%{pkgdir}/libgfal_plugin_xrootd.so
%{_pkgdocdir}/*

%changelog
* Tue Oct 22 2013 Alejandro Alvarez <aalvarez at cern.ch> - 0.2.2-3
 - Added configuration file

* Wed May 08 2013 Adrien Devresse <adevress at cern.ch> - 0.2.2-2
 - First EPEL compatible version from review comments


