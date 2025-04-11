<?php

namespace App\Classes\LDAP\Attribute\Password;

final class Argon2i extends Base
{
	public const key = 'ARGON2';
	protected const subkey = 'argon2i';
	protected const identifier = '$argon2i';

	public static function subid(string $password): bool
	{
		return str_starts_with(base64_decode(self::password($password)),self::identifier.'$');
	}

	public function compare(string $source,string $compare): bool
	{
		return password_verify($compare,base64_decode($this->password($source)));
	}

	public function encode(string $password): string
	{
		return sprintf('{%s}%s',self::key,base64_encode(password_hash($password,PASSWORD_ARGON2I)));
	}
}