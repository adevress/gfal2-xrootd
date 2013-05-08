Name:           gfal2-plugin-xrootd
Version:        0.2.2
Release:        2%{?dist}
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
* Wed May 08 2013 Adrien Devresse <adevress at cern.ch> - 0.2.2-2
 - First EPEL compatible version from review comments


