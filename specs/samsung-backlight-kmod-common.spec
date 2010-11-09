Name:		samsung-backlight-kmod-common
Version:	svn20101109
Release:	1%{?dist}
Summary:	Empty package for samsung-backlight-kmod

Group:		System Environment/Kernel
License:	GPLv2+
URL:		https://github.com/gregkh/samsung-backlight
#Source0:	samsung-backlight.c
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildArch:	noarch
BuildRequires:	%{_bindir}/kmodtool
Requires:       samsung-backlight-kmod >= %{version}

%description
This package is empty required for Samsung Laptops Backlight driver

%prep

%build

%install
#install -D -m 644 %{SOURCE0} $RPM_BUILD_ROOT/%{_docdir}/%{SOURCE0}

%files
#%defattr(-,root,root)
#%doc %{SOURCE0}

%clean

%changelog
* Tue Nov  9 2010  Alexei Panov <elemc@atisserv.ru> - svn20101109-1
- Initial build. The patch add install instructions.
