<div class="card card-solid">
	<div class="card-body">
		<div class="row">
			<div class="col-12">
				<h3 class="d-inline-block">DEBUG Information</h3>

				<table class="table">
					<thead>
					<tr>
						<th>Setting</th>
						<th>Value</th>
					</tr>
					</thead>

					<tbody>
					<!-- User Logged In -->
					<tr>
						<td>User</td>
						<td>{{ $user }}</td>
					</tr>

					<!-- Base DNs -->
					<tr>
						<td>BaseDN(s)</td>
						<td>
							<table class="table table-sm table-borderless">
								@foreach($server->baseDNs()->sort(fn($item)=>$item->sort_key) as $item)
									<tr>
										<td class="ps-0">{{ $item->getDn() }}</td>
									</tr>
								@endforeach
							</table>
						</td>
					</tr>

					<!-- Schema DN -->
					<tr>
						<td>Schema DN</td>
						<td>{{ $server->schemaDN() }}</td>
					</tr>

					<!-- Schema DN -->
					<tr>
						<td>Root URL</td>
						<td>{{ request()->root() }}</td>
					</tr>
					</tbody>
				</table>
			</div>
		</div>
	</div>
</div>