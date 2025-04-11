<?php

namespace App\Ldap;

use LdapRecord\Configuration\DomainConfiguration;
use LdapRecord\Connection as ConnectionBase;
use LdapRecord\LdapInterface;

class Connection extends ConnectionBase
{

	public function __construct(DomainConfiguration|array $config=[],?LdapInterface $ldap=NULL)
	{
		parent::__construct($config,$ldap);

		// We need to override this so that we use our own Guard, that stores the users credentials in the session
		$this->authGuardResolver = function () {
			return new Guard($this->ldap, $this->configuration);
		};
	}
}